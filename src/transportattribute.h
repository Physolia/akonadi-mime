/*
    SPDX-FileCopyrightText: 2009 Constantin Berzan <exit3219@gmail.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-mime_export.h"

#include <Akonadi/Attribute>

#include <memory>

namespace Akonadi
{
class TransportAttributePrivate;

/**
  Attribute determining which transport to use for sending a message.

  @see mailtransport
  @see TransportManager.

  @author Constantin Berzan <exit3219@gmail.com>
  @since 4.4
*/
class AKONADI_MIME_EXPORT TransportAttribute : public Akonadi::Attribute
{
public:
    /**
      Creates a new TransportAttribute.
    */
    explicit TransportAttribute(int id = -1);

    /**
      Destroys this TransportAttribute.
    */
    ~TransportAttribute() override;

    /* reimpl */
    TransportAttribute *clone() const override;
    QByteArray type() const override;
    QByteArray serialized() const override;
    void deserialize(const QByteArray &data) override;

    /**
      Returns the transport id to use for sending this message.
      @see TransportManager.
    */
    [[nodiscard]] int transportId() const;

    /**
      Sets the transport id to use for sending this message.
    */
    void setTransportId(int id);

private:
    std::unique_ptr<TransportAttributePrivate> const d;
};
} // namespace MailTransport
