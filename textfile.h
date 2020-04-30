#include <QObject>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QFileInfo>
#include <QJSEngine>
#include <QTextStream>

namespace Qbs4QJS {

class TextFile : public QObject
{
	Q_OBJECT

public:
	enum OpenMode {
		ReadOnly = QIODevice::ReadOnly,
    WriteOnly = QIODevice::WriteOnly,
    ReadWrite = QIODevice::ReadWrite,
    Append = QIODevice::Append
	};
	Q_ENUM(OpenMode)

	Q_INVOKABLE TextFile(const QString &filePath) {
		m_file = new QFile(filePath);	
	}
	
	~TextFile() override {
		close();
	}

	Q_INVOKABLE void open (OpenMode mode = ReadOnly) {
		m_stream = new QTextStream(m_file);
		if (!m_file->open((QIODevice::OpenModeFlag)mode)){
			qjsEngine(this)->throwError(m_file->errorString());
			delete m_stream;
			delete m_file;
			m_stream = nullptr;
			m_file = nullptr;
		}
	}

	Q_INVOKABLE bool atEof() const {
		if (!openCheck()){
			return false;
		}
		return m_stream->atEnd();
	}

	Q_INVOKABLE void close() {
		delete m_stream;
    m_stream = nullptr;
    m_file->close();
    delete m_file;
    m_file = nullptr;
	}

	Q_INVOKABLE QString filePath() const {
		return QFileInfo(*m_file).absoluteFilePath();
	}

	Q_INVOKABLE QString readAll() const {
		if (!openCheck()) {
			return "";
		}
		return m_stream->readAll();
	}

	Q_INVOKABLE QString readLine() const {
		return m_stream->readLine();
	}

	Q_INVOKABLE void setCodec(QString &codec) {
		m_stream->setCodec(qPrintable(codec));
	}

	Q_INVOKABLE void truncate() {
		m_file->resize(0);
    m_stream->reset();
	}

	Q_INVOKABLE void write(const QString &str) {
		if (openCheck()) {
			(*m_stream) << str;
		}
	}

	Q_INVOKABLE void writeLine(const QString &str) {
		if (openCheck()) {
			(*m_stream) << str << '\n'; // TODO: check for windows for \r ?
		}
	}
private:
	QFile *m_file = nullptr;
  QTextStream *m_stream = nullptr;

	bool openCheck() const {
		if (!m_file){
			qjsEngine(this)->throwError("File is not open.");
			return false;
		}
		return true;
	}
};

} // end namespace Qbs4QJS