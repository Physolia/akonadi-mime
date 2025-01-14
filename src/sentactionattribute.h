/*
  SPDX-FileCopyrightText: 2010 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.net>
  SPDX-FileContributor: Tobias Koenig <tokoe@kdab.com>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/Attribute>
#include <QList>
#include <QSharedDataPointer>
#include <QVariant>

#include <memory>

namespace Akonadi
{
class SentActionAttributePrivate;
class SentActionAttributeActionPrivate;

/**
 * @short An Attribute that stores the action to execute after sending.
 *
 * This attribute stores the action that will be executed by the mail dispatcher
 * after a mail has successfully be sent.
 *
 * @author Tobias Koenig <tokoe@kdab.com>
 * @since 4.6
 */
class AKONADI_MIME_EXPORT SentActionAttribute : public Akonadi::Attribute
{
public:
    /**
     * @short A sent action.
     */
    class AKONADI_MIME_EXPORT Action
    {
    public:
        /**
         * Describes the action type.
         */
        enum Type {
            Invalid, ///< An invalid action.
            MarkAsReplied, ///< The message will be marked as replied.
            MarkAsForwarded ///< The message will be marked as forwarded.
        };

        /**
         * Describes a list of sent actions.
         */
        using List = QList<Action>;

        /**
         * Creates a new invalid action.
         */
        Action();

        /**
         * Creates a new action.
         *
         * @param type The type of action that shall be executed.
         * @param value The action specific argument.
         */
        Action(Type type, const QVariant &value);

        /**
         * Creates an action from an @p other action.
         */
        Action(const Action &other);

        /**
         * Destroys the action.
         */
        ~Action();

        /**
         * Returns the type of the action.
         */
        Type type() const;

        /**
         * Returns the argument value of the action.
         */
        QVariant value() const;

        /**
         * @internal
         */
        Action &operator=(const Action &other);

        /**
         * @internal
         */
        bool operator==(const Action &other) const;

    private:
        //@cond PRIVATE
        QSharedDataPointer<SentActionAttributeActionPrivate> d;
        //@endcond
    };

    /**
     * Creates a new sent action attribute.
     */
    explicit SentActionAttribute();

    /**
     * Destroys the sent action attribute.
     */
    ~SentActionAttribute() override;

    /**
     * Adds a new action to the attribute.
     *
     * @param type The type of the action that shall be executed.
     * @param value The action specific argument.
     */
    void addAction(Action::Type type, const QVariant &value);

    /**
     * Returns the list of actions.
     */
    Action::List actions() const;

    /* reimpl */
    SentActionAttribute *clone() const override;
    QByteArray type() const override;
    QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

private:
    //@cond PRIVATE
    std::unique_ptr<SentActionAttributePrivate> const d;
    //@endcond
};
}
Q_DECLARE_TYPEINFO(Akonadi::SentActionAttribute::Action, Q_RELOCATABLE_TYPE);
