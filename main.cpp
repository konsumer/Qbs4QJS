#include<iostream>

#include <QCoreApplication>
#include <QQmlEngine>
#include <QFile>
#include <QTextCodec>
#include <QByteArray>
#include <QString>
#include <QDebug>

#include "binaryfile.h"

using namespace std;
int main(int argc, char *argv[])
{
	if (argc < 2) {
		cerr << "Usage: Qbs4QJS <script.js>\n";
		return 1;
	}
	
	QCoreApplication app(argc, argv);
	QQmlEngine jsEngine;

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
