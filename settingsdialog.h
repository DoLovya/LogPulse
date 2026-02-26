#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFontComboBox>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QSettings>

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget* parent = nullptr);

	QFont currentFont() const;
	int currentFontSize() const;

private slots:
	void saveSettings();

private:
	QFontComboBox* fontComboBox;
	QSpinBox* fontSizeSpinBox;
	QDialogButtonBox* buttonBox;
};

#endif // SETTINGSDIALOG_H
