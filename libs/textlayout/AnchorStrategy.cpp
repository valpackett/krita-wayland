/* This file is part of the KDE project
 * Copyright (C) 2007, 2009, 2010 Thomas Zander <zander@kde.org>
 * Copyright (C) 2011 Matus Hanzes <matus.hanzes@ixonos.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "AnchorStrategy.h"

#include "KoTextShapeContainerModel.h"
#include "KoTextDocumentLayout.h"
#include "KoTextLayoutRootArea.h"

#include <KoShapeContainer.h>
#include <KoTextShapeData.h>

#include <kdebug.h>



AnchorStrategy::AnchorStrategy(KoTextAnchor *anchor, KoTextLayoutRootArea *rootArea)
        : m_model(0)
        , m_anchor(anchor)
        , m_rootArea(rootArea)
        , m_pageRect(0,0,10,10)
        , m_pageContentRect(0,0,10,10)
        , m_paragraphRect(0,0,0,0)
        , m_pageNumber(0)
{
}

AnchorStrategy::~AnchorStrategy()
{
    if (m_model)
        m_model->removeAnchor(m_anchor);
}

void AnchorStrategy::detachFromModel()
{
    m_model = 0;
}

QRectF AnchorStrategy::pageRect()
{
    return m_pageRect;
}

void AnchorStrategy::setPageRect(const QRectF &pageRect)
{
    m_pageRect = pageRect;
}

QRectF AnchorStrategy::pageContentRect()
{
    return m_pageContentRect;
}

void AnchorStrategy::setPageContentRect(QRectF &pageContentRect)
{
    m_pageContentRect = pageContentRect;
}

QRectF AnchorStrategy::paragraphRect()
{
    return m_paragraphRect;
}

void AnchorStrategy::setParagraphRect(const QRectF &paragraphRect)
{
    m_paragraphRect = paragraphRect;
}

int AnchorStrategy::pageNumber()
{
    return m_pageNumber;
}

void AnchorStrategy::setPageNumber(int pageNumber)
{
    m_pageNumber = pageNumber;
}


/// as multiple shapes can hold 1 text flow; the anchored shape can be moved between containers and thus models
void AnchorStrategy::updatePosition(KoShape *shape, const QTextDocument *document, int posInDocument)
{
    Q_UNUSED(document);
    Q_UNUSED(posInDocument);

    KoShapeContainer *container = dynamic_cast<KoShapeContainer*>(m_rootArea->associatedShape());
    if (container == 0) {
        if (m_model)
            m_model->removeAnchor(m_anchor);
        m_model = 0;
        shape->setParent(0);
        return;
    }
    KoTextShapeContainerModel *theModel = dynamic_cast<KoTextShapeContainerModel*>(container->model());
    if (theModel != m_model) {
        if (m_model)
            m_model->removeAnchor(m_anchor);
        if (shape->parent() != container) {
            if (shape->parent()) {
                shape->parent()->removeShape(shape);
            }
            container->addShape(shape);
        }
        m_model = theModel;
        m_model->addAnchor(m_anchor);
    }
    Q_ASSERT(m_model == theModel);
}
