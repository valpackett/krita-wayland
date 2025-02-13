/*
 *  SPDX-FileCopyrightText: 2021 Emmet O 'Neill <emmetoneill.pdx@gmail.com>
 *  SPDX-FileCopyrightText: 2021 Eoin O 'Neill <eoinoneill1991@gmail.com>
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KISPOPUPWIDGETINTERFACE_H
#define KISPOPUPWIDGETINTERFACE_H

#include <QWidget>
#include <QGridLayout>

#include "kis_debug.h"
#include "kis_assert.h"

#include "KoCanvasBase.h"
#include "input/kis_input_manager.h"

/**
 * @brief The PopupWidgetInterface abstract class defines
 * the basic interface that will be used by all popup widgets.
 */
class KisPopupWidgetInterface {
public:
    virtual ~KisPopupWidgetInterface() {}
    virtual void popup(const QPoint& position) = 0;
};


/**
 * @brief The KisPopupWidget class is a simple wrapper that
 * turns any QWidget into a popup widget that can be temporarily
 * displayed over the canvas.
 */
class KisPopupWidget : public QWidget, public KisPopupWidgetInterface
{
public:
    KisPopupWidget(QWidget* toPopup, KoCanvasBase* canvas)
        : QWidget(canvas->canvasWidget())
    {
        KIS_ASSERT(toPopup);

        m_toPopup = toPopup;
        m_toPopup->setParent(this);

        setLayout(new QGridLayout());
        layout()->addWidget(m_toPopup);

        setAutoFillBackground(true);
    }

    void popup(const QPoint& position) override {
        setVisible(true);
        adjustPopupLayout(position);
    }

    void adjustPopupLayout(const QPoint& position) {
        if (isVisible() && parentWidget())  {
            const float widgetMargin = -20.0f;
            const QRect fitRect = kisGrowRect(parentWidget()->rect(), widgetMargin);
            const QPoint paletteCenterOffset(sizeHint().width() / 2, sizeHint().height() / 2);

            QRect paletteRect = rect();

            paletteRect.moveTo(position - paletteCenterOffset);

            paletteRect = kisEnsureInRect(paletteRect, fitRect);
            move(paletteRect.topLeft());
        }
    }

    QSize sizeHint() const override {
        KIS_ASSERT(m_toPopup);
        return m_toPopup->sizeHint();
    }

private:
    QWidget* m_toPopup;
};

#endif // KISPOPUPWIDGETINTERFACE_H
