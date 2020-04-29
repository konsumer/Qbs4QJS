#include<iostream>

#include <QCoreApplication>
#include <QJSEngine>
#include <QFile>
#include <QString>
#include <QDebug>

#include "binaryfile.h"
#include "environment.h"
#include "file.h"
#include "fileinfo.h"
#include "process.h"
#include "propertylist.h"
#include "temporarydir.h"

using namespace std;
int main(int argc, char *argv[])
{
	if (argc < 2) {
		cerr << "Usage: Qbs4QJS <script.js>\n";
		return 1;
	}
	
	QCoreApplication app(argc, argv);
	QJSEngine jsEngine;
	jsEngine.installExtensions(QJSEngine::AllExtensions);

	QString fileName = argv[1];
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly)){
		qDebug() << "Could not open file " << fileName;
		return 1;
	}
	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();

	jsEngine.globalObject().setProperty("BinaryFile", jsEngine.newQMetaObject(&Qbs4QJS::BinaryFile::staticMetaObject));
	jsEngine.globalObject().setProperty("Environment", jsEngine.newQObject(new Qbs4QJS::Environment()));
	jsEngine.globalObject().setProperty("File", jsEngine.newQObject(new Qbs4QJS::File()));
	jsEngine.globalObject().setProperty("FileInfo", jsEngine.newQObject(new Qbs4QJS::FileInfo()));
	jsEngine.globalObject().setProperty("Process", jsEngine.newQMetaObject(&Qbs4QJS::Process::staticMetaObject));
	jsEngine.globalObject().setProperty("PropertyList", jsEngine.newQMetaObject(&Qbs4QJS::PropertyList::staticMetaObject));
	jsEngine.globalObject().setProperty("TemporaryDir", jsEngine.newQMetaObject(&Qbs4QJS::TemporaryDir::staticMetaObject));

	QJSValue result = jsEngine.evaluate(contents, fileName);

	if (result.isError()) {
		qDebug()
			<< "Uncaught exception at line"
			<< result.property("lineNumber").toInt()
			<< ":" << result.toString();
		return 1;
	}

	return 0;
}
