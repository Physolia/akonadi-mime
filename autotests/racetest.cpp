/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "racetest.h"

#include <KProcess>
#include <QDebug>

#include <Akonadi/AgentInstance>
#include <Akonadi/AgentInstanceCreateJob>
#include <Akonadi/AgentManager>
#include <Akonadi/AgentType>
#include <akonadi/qtest_akonadi.h>
#include <control.h>
//#include <localfolders.h>

#define TIMEOUT_SECONDS 20
#define MAXCOUNT 10
// NOTE: REQUESTER_EXE is defined by cmake.

Q_DECLARE_METATYPE(QProcess::ProcessError)
Q_DECLARE_METATYPE(QProcess::ExitStatus)

using namespace Akonadi;

void RaceTest::initTestCase()
{
    QVERIFY(Control::start());
    QTest::qWait(1000); // give the MDA time to start so that we can kill it in peace
    qRegisterMetaType<QProcess::ProcessError>();
    qRegisterMetaType<QProcess::ExitStatus>();
}

void RaceTest::testMultipleProcesses_data()
{
    QTest::addColumn<int>("count"); // how many processes to create
    QTest::addColumn<int>("delay"); // number of ms to wait before starting next process

    QTest::newRow("1-nodelay") << 1 << 0;
    QTest::newRow("2-nodelay") << 2 << 0;
    QTest::newRow("5-nodelay") << 5 << 0;
    QTest::newRow("10-nodelay") << 10 << 0;
    QTest::newRow("2-shortdelay") << 2 << 100;
    QTest::newRow("5-shortdelay") << 5 << 100;
    QTest::newRow("10-shortdelay") << 10 << 100;
    QTest::newRow("2-longdelay") << 2 << 1000;
    QTest::newRow("5-longdelay") << 5 << 1000;
    QTest::newRow("5-verylongdelay") << 5 << 4000;
    Q_ASSERT(10 <= MAXCOUNT);
}

void RaceTest::testMultipleProcesses()
{
    QFETCH(int, count);
    QFETCH(int, delay);

    killZombies();

    // Remove all maildir instances (at most 1 really) and MDAs (which use LocalFolders).
    // (This is to ensure that one of *our* instances is the main instance.)
    AgentType::List types;
    types.append(AgentManager::self()->type(QLatin1String("akonadi_maildir_resource")));
    types.append(AgentManager::self()->type(QLatin1String("akonadi_maildispatcher_agent")));
    AgentInstance::List instances = AgentManager::self()->instances();
    for (const AgentInstance &instance : std::as_const(instances)) {
        if (types.contains(instance.type())) {
            qDebug() << "Removing instance of type" << instance.type().identifier();
            AgentManager::self()->removeInstance(instance);
            QSignalSpy removedSpy(AgentManager::self(), SIGNAL(instanceRemoved(Akonadi::AgentInstance)));
            QVERIFY(removedSpy.wait());
        }
    }
    instances = AgentManager::self()->instances();
    for (const AgentInstance &instance : std::as_const(instances)) {
        QVERIFY(!types.contains(instance.type()));
    }

    QSignalSpy *errorSpy[MAXCOUNT];
    QSignalSpy *finishedSpy[MAXCOUNT];
    for (int i = 0; i < count; i++) {
        qDebug() << "Starting process" << i + 1 << "of" << count;
        KProcess *proc = new KProcess;
        procs.append(proc);
        proc->setProgram(QStringLiteral(REQUESTER_EXE));
        errorSpy[i] = new QSignalSpy(proc, SIGNAL(error(QProcess::ProcessError)));
        finishedSpy[i] = new QSignalSpy(proc, SIGNAL(finished(int, QProcess::ExitStatus)));
        proc->start();
        QTest::qWait(delay);
    }
    qDebug() << "Launched" << count << "processes.";

    int seconds = 0;
    int error, finished;
    while (true) {
        seconds++;
        QTest::qWait(1000);

        error = 0;
        finished = 0;
        for (int i = 0; i < count; i++) {
            if (errorSpy[i]->count() > 0) {
                error++;
            }
            if (finishedSpy[i]->count() > 0) {
                finished++;
            }
        }
        qDebug() << seconds << "seconds elapsed." << error << "processes error'd," << finished << "processes finished.";

        if (error + finished >= count) {
            break;
        }

#if 0
        if (seconds >= TIMEOUT_SECONDS) {
            qDebug() << "Timeout, gdb master!";
            QTest::qWait(1000 * 1000);
        }
#endif
        QVERIFY2(seconds < TIMEOUT_SECONDS, "Timeout");
    }

    QCOMPARE(error, 0);
    QCOMPARE(finished, count);
    for (int i = 0; i < count; i++) {
        qDebug() << "Checking exit status of process" << i + 1 << "of" << count;
        QCOMPARE(finishedSpy[i]->count(), 1);
        QList<QVariant> args = finishedSpy[i]->takeFirst();
        if (args[0].toInt() != 2) {
            qDebug() << "Exit status" << args[0].toInt() << ", expected 2. Timeout, gdb master!";
            QTest::qWait(1000 * 1000);
        }
        QCOMPARE(args[0].toInt(), 2);
    }

    while (!procs.isEmpty()) {
        KProcess *proc = procs.takeFirst();
        QCOMPARE(proc->exitStatus(), QProcess::NormalExit);
        QCOMPARE(proc->exitCode(), 2);
        delete proc;
    }
    QVERIFY(procs.isEmpty());
}

void RaceTest::killZombies()
{
    while (!procs.isEmpty()) {
        // These processes probably hung, and will never recover, so we need to kill them.
        // (This happens if the last test failed.)
        qDebug() << "Killing zombies from the past.";
        KProcess *proc = procs.takeFirst();
        proc->kill();
        proc->deleteLater();
    }
}

QTEST_AKONADIMAIN(RaceTest)

#include "moc_racetest.cpp"
