#pragma once

#include <QLineEdit>

class QTimer;

class SearchBar : public QLineEdit {
    Q_OBJECT
public:
    explicit SearchBar(QWidget *parent = nullptr);

signals:
    void queryChanged(const QString &query);

private:
    QTimer *m_debounce = nullptr;
    static constexpr int kDebounceMs = 150;
};
