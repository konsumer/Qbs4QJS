#include <QObject>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QJSEngine>

namespace Qbs4QJS {

class BinaryFile : public QObject
{
	Q_OBJECT

public:
	Q_ENUM(QIODevice::OpenModeFlag)

	Q_INVOKABLE BinaryFile(const QString &filePath) {
		m_file = new QFile(filePath);	
	}
	
	~BinaryFile() override {
		close();
	}

	Q_INVOKABLE QString filePath() const {
		return QFileInfo(*m_file).absoluteFilePath();
	}

	Q_INVOKABLE bool atEof() const {
		return m_file->atEnd();
	}

	Q_INVOKABLE qint64 size() const {
		return m_file->size();
	}

	Q_INVOKABLE void open (QIODevice::OpenModeFlag mode = QIODevice::ReadOnly) {
		if (!m_file->open(mode)){
			QJSEngine(this).throwError(m_file->errorString());
		}
	}

	Q_INVOKABLE qint64 pos() const {
		return m_file->pos();
	}

	Q_INVOKABLE void resize(qint64 size) {
		m_file->resize(size);
	}

	Q_INVOKABLE void seek(qint64 pos) {
		m_file->seek(pos);
	}

	Q_INVOKABLE QByteArray read(qint64 size) {
		return m_file->read(size);
	}

	Q_INVOKABLE void write(const QByteArray &data) {
		m_file->write(data);
	}

	Q_INVOKABLE void close() {
		m_file->close();
		delete m_file;
		m_file = nullptr;
	}

private:
	QFile *m_file = nullptr;
};

} // end namespace Qbs4QJS