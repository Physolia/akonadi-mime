/*
  SPDX-FileCopyrightText: 2014-2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class NewMailNotifierAttributeTest : public QObject
{
    Q_OBJECT
public:
    explicit NewMailNotifierAttributeTest(QObject *parent = nullptr);

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldSetIgnoreNotification();
    void shouldSerializedData();
    void shouldCloneAttribute();
    void shouldHaveType();
};

