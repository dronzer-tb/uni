#include "SearchBar.h"

#include <QTimer>

SearchBar::SearchBar(QWidget *parent) : QLineEdit(parent) {
    setObjectName("SearchBar");
    setPlaceholderText("Search packages — apt · flatpak · snap · github");
    setClearButtonEnabled(true);

    m_debounce = new QTimer(this);
    m_debounce->setSingleShot(true);
    m_debounce->setInterval(kDebounceMs);

    connect(this, &QLineEdit::textChanged, this, [this] { m_debounce->start(); });
    connect(m_debounce, &QTimer::timeout, this,
            [this] { emit queryChanged(text()); });
}
