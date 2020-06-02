
#ifndef datathreadtest_H
#define datathreadtest_H

// Qt include
#include <QtCore>

// To run some code in a new thread, 
// subclass QThread and reimplement its run() function
class Thread : public QThread
{
private:
	// QThread class method
	void run() final override
	{
		qDebug()<<"From worker thread: "<<currentThreadId();
	}
};

// some test
// int main(int argc, char *argv[])
// {
// 	QCoreApplication a(argc, argv);
// 	qDebug()<<"From main thread: "<<QThread::currentThreadId();
// 
// 	Thread t;
// 	QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));
// 
// 	t.start();
// 	return a.exec();
// }

#endif // Include guard