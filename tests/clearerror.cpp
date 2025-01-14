/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "clearerror.h"

#include <QApplication>

#include <Akonadi/Collection>
#include <Akonadi/Control>
#include <Akonadi/DispatcherInterface>
#include <Akonadi/SpecialMailCollections>
#include <Akonadi/SpecialMailCollectionsRequestJob>
#include <filteractionjob_p.h>

using namespace Akonadi;
using namespace Akonadi;

Runner::Runner()
{
    Control::start();

    auto rjob = new SpecialMailCollectionsRequestJob(this);
    rjob->requestDefaultCollection(SpecialMailCollections::Outbox);
    connect(rjob, &SpecialMailCollectionsRequestJob::result, this, &Runner::checkFolders);
    rjob->start();
}

void Runner::checkFolders()
{
    DispatcherInterface().retryDispatching();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("clearerror"));

    new Runner();
    return app.exec();
}

#include "moc_clearerror.cpp"
