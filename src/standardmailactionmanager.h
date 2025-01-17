/*
    This file is part of Akonadi Mail.

    Copyright (c) 2009 - 2010 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/StandardActionManager>

#include <QObject>

#include <memory>

class QAction;
class KActionCollection;
class QItemSelectionModel;
class QWidget;

namespace Akonadi
{
class Item;
class StandardMailActionManagerPrivate;

/**
 * @short Manages emails specific actions for collection and item views.
 *
 * @author Andras Mantia <amantia@kde.org>
 * @since 4.6
 */
class AKONADI_MIME_EXPORT StandardMailActionManager : public QObject
{
    Q_OBJECT

public:
    /**
     * Describes the supported actions.
     */
    enum Type {
        MarkMailAsRead = StandardActionManager::LastType + 1, ///< Marks a mail as read
        MarkMailAsUnread, ///< Marks a mail as unread
        MarkMailAsImportant, ///< Marks a mail as important
        MarkMailAsActionItem, ///< Marks a mail as action item
        MarkAllMailAsRead, ///< Marks all mails in a folder as read.
        MarkAllMailAsReadRecursive, ///< Marks all mails in a folder and its subfolders as read.
        MarkAllMailAsUnread, ///< Marks all mails in a folder as unread.
        MarkAllMailAsImportant, ///< Marks all mails in a folder as important
        MarkAllMailAsActionItem, ///< Marks all mails in a folder as action item
        MoveToTrash, ///< Move all selected messages and folders to trash.
        MoveAllToTrash, ///< Move all messages of the current folder to trash.
        RemoveDuplicates, ///< Removes all duplicated messages.
        EmptyAllTrash, ///< Empties trash folders on all accounts
        EmptyTrash, ///< Empties the trash folder, if a trash folder was selected
        LastType ///< Marks last action.
    };

    /**
     * Creates a new standard mail action manager.
     *
     * @param actionCollection The action collection to operate on.
     * @param parent The parent widget.
     */
    explicit StandardMailActionManager(KActionCollection *actionCollection, QWidget *parent = nullptr);

    /**
     * Destroys the standard mail action manager.
     */
    ~StandardMailActionManager() override;

    /**
     * Sets the collection selection model based on which the collection
     * related actions should operate. If none is set, all collection actions
     * will be disabled.
     *
     * @param selectionModel selection model for collections
     */
    void setCollectionSelectionModel(QItemSelectionModel *selectionModel);

    /**
     * Sets the item selection model based on which the item related actions
     * should operate. If none is set, all item actions will be disabled.
     *
     * @param selectionModel selection model for items
     */
    void setItemSelectionModel(QItemSelectionModel *selectionModel);

    /**
     * Creates the action of the given type and adds it to the action collection
     * specified in the constructor if it does not exist yet. The action is
     * connected to its default implementation provided by this class.
     *
     * @param type action type
     */
    QAction *createAction(Type type);

    /**
     * Creates the action of the given type and adds it to the action collection
     * specified in the constructor if it does not exist yet. The action is
     * connected to its default implementation provided by this class.
     * @param type action type
     */
    QAction *createAction(StandardActionManager::Type type);

    /**
     * Convenience method to create all standard actions.
     * @see createAction()
     */
    void createAllActions();

    /**
     * Returns the action of the given type, 0 if it has not been created (yet).
     * @param type action type
     */
    QAction *action(Type type) const;

    /**
     * Returns the action of the given type, 0 if it has not been created (yet).
     * @param type action type
     */
    QAction *action(StandardActionManager::Type type) const;

    /**
     * Sets the label of the action @p type to @p text, which is used during
     * updating the action state and substituted according to the number of
     * selected objects. This is mainly useful to customize the label of actions
     * that can operate on multiple objects.
     *
     * Example:
     * @code
     * acctMgr->setActionText( Akonadi::StandardActionManager::CopyItems,
     *                         ki18np( "Copy Item", "Copy %1 Items" ) );
     * @endcode
     *
     * @param type action type
     * @param text localized text for action
     */
    void setActionText(StandardActionManager::Type type, const KLocalizedString &text);

    /**
     * Sets whether the default implementation for the given action @p type
     * shall be executed when the action is triggered.
     *
     * @param intercept If @c false, the default implementation will be executed,
     *                  if @c true no action is taken.
     */
    void interceptAction(Type type, bool intercept = true);

    /**
     * Sets whether the default implementation for the given action @p type
     * shall be executed when the action is triggered.
     *
     * @param type action type
     * @param intercept If @c false, the default implementation will be executed,
     *                  if @c true no action is taken.
     */
    void interceptAction(StandardActionManager::Type type, bool intercept = true);

    /**
     * Returns the list of collections that are currently selected.
     * The list is empty if no collection is currently selected.
     */
    [[nodiscard]] Akonadi::Collection::List selectedCollections() const;

    /**
     * Returns the list of items that are currently selected.
     * The list is empty if no item is currently selected.
     */
    [[nodiscard]] Akonadi::Item::List selectedItems() const;

    /**
     * Sets the favorite collections model based on which the collection
     * relatedactions should operate. If none is set, the "Add to Favorite Folders" action
     * will be disabled.
     *
     * @param favoritesModel model for a user's favorite mail collections
     */
    void setFavoriteCollectionsModel(FavoriteCollectionsModel *favoritesModel);

    /**
     * Sets the favorite collection selection model based on which the favorite
     * collection related actions should operate. If none is set, all favorite modifications
     * actions will be disabled.
     *
     * @param selection model for favorite collections
     */
    void setFavoriteSelectionModel(QItemSelectionModel *selectionModel);

    void setCollectionPropertiesPageNames(const QStringList &names);

    Akonadi::StandardActionManager *standardActionManager() const;

    void markItemsAs(const QByteArray &typeStr, const Akonadi::Item::List &items, bool checkIntercept = true);
    void markAllItemsAs(const QByteArray &typeStr, const Akonadi::Collection::List &collections, bool checkIntercept = true);

    void setItems(const Item::List &selectedItems);

Q_SIGNALS:
    /**
     * This signal is emitted whenever the action state has been updated.
     * In case you have special needs for changing the state of some actions,
     * connect to this signal and adjust the action state.
     */
    void actionStateUpdated();

private:
    //@cond PRIVATE
    friend class StandardMailActionManagerPrivate;
    std::unique_ptr<StandardMailActionManagerPrivate> const d;
    //@endcond
};
}
