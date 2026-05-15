#include "PackageDetailPane.h"

#include "Theme.h"

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

PackageDetailPane::PackageDetailPane(QWidget *parent) : QWidget(parent) {
    setObjectName("PackageDetailPane");
    buildUi();
}

void PackageDetailPane::buildUi() {
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(12);

    m_name = new QLabel(this);
    m_name->setObjectName("DetailName");
    m_name->setWordWrap(true);

    m_version = new QLabel(this);
    m_version->setObjectName("DetailVersion");

    m_description = new QLabel(this);
    m_description->setObjectName("DetailDescription");
    m_description->setWordWrap(true);

    m_meta = new QLabel(this);
    m_meta->setObjectName("DetailMeta");
    m_meta->setWordWrap(true);

    auto *divider = new QFrame(this);
    divider->setFrameShape(QFrame::HLine);
    divider->setObjectName("DetailDivider");

    m_sourcePicker = new QComboBox(this);
    m_sourcePicker->setObjectName("SourcePicker");

    m_installBtn = new QPushButton("Install", this);
    m_installBtn->setObjectName("InstallButton");

    connect(m_installBtn, &QPushButton::clicked, this, [this] {
        const QString name = m_current.value("name").toString();
        const QString source = m_sourcePicker->currentData().toString();
        if (!name.isEmpty()) emit installRequested(name, source);
    });

    root->addWidget(m_name);
    root->addWidget(m_version);
    root->addWidget(m_description);
    root->addWidget(divider);
    root->addWidget(m_meta);
    root->addStretch(1);
    root->addWidget(m_sourcePicker);
    root->addWidget(m_installBtn);
}

void PackageDetailPane::setPackage(const QJsonObject &pkg) {
    m_current = pkg;
    m_name->setText(pkg.value("name").toString());
    m_version->setText(pkg.value("version").toString());
    m_description->setText(pkg.value("description").toString());

    const QString backend = pkg.value("backend").toString();
    const auto size = pkg.value("size_bytes").toVariant().toLongLong();
    const int rank = pkg.value("source_speed_rank").toInt(99);

    QString sizeStr;
    if (size > 0) {
        const double mb = size / (1024.0 * 1024.0);
        sizeStr = QStringLiteral(" · %1 MB").arg(QString::number(mb, 'f', 1));
    }
    m_meta->setText(QStringLiteral("Backend: %1   Speed rank: %2%3")
                        .arg(backend.toUpper())
                        .arg(rank == 99 ? QStringLiteral("?") : QString::number(rank))
                        .arg(sizeStr));

    m_sourcePicker->clear();
    if (!backend.isEmpty())
        m_sourcePicker->addItem(QStringLiteral("Install via %1").arg(backend.toUpper()), backend);
    // M4: when multiple sources for the same package exist, list them all here.
}
