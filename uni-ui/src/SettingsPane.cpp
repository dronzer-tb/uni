#include "SettingsPane.h"

#include <QLabel>
#include <QVBoxLayout>

SettingsPane::SettingsPane(QWidget *parent) : QWidget(parent) {
    setObjectName("SettingsPane");
    auto *root = new QVBoxLayout(this);
    auto *label = new QLabel("Settings (coming in M5)", this);
    root->addWidget(label);
    root->addStretch(1);
}
