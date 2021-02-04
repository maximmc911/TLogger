#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QTime>
#include <QFile>

#include <iostream>
#include <string>

extern QString results_dir;  // имя директории для сохранения файлов xml
extern QString results_calibr_dir; //для калибровок
extern QString result_log;

namespace logs {
// tests additions
namespace ta {
    inline const QString l_cm(const QVariant & comm)
    {return "_cm:" + comm.toString();}
    inline const QString l_st(const QVariant & style)
    {return "_st:" + style.toString();}
    inline const QString l_min(double val)
    {return "_min:" + QString::number(val);}
    inline const QString l_max(double val)
    {return "_max:" + QString::number(val);}
    inline const QString l_num(QVariant val)
    { return QString::number(val.toDouble(), 'f', 2).remove(QRegExp("0+$")).remove(QRegExp("\\.$")); }
    inline const QString l_time(QString form = "hh:mm:ss", bool enbl_tag = true)
    {return enbl_tag ? "_t:" :" " + QTime::currentTime().toString(form);}
    #define              l_func() \
    QString(QString(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__) + ":" + QString::number(__LINE__) + "::" + __func__ + "()")

    // пресеты стилей
    inline const QString l_blue(int pos = 0)
    {return l_st(l_num(pos) + ", background: rgb(50, 150, 255);");}
    inline const QString l_green(int pos = 0)
    {return l_st(l_num(pos) + ", background: rgb(50, 255, 120);");}
    inline const QString l_orange(int pos = 0)
    {return l_st(l_num(pos) + ", background: rgb(255, 130, 50);");}
    inline const QString l_yellow(int pos = 0)
    {return l_st(l_num(pos) + ", background: rgb(255, 205, 50);");}
    inline const QString l_red(int pos = 0)
    {return l_st(l_num(pos) + ", background: rgb(255, 90, 90);");}
}

namespace log_flags {
    struct _console {};
    struct _end {};
}

extern const log_flags::_end     end;         // Переводит строку в таблице тестовых логов (остальные логи не нуждаются в данном флаге)
extern const log_flags::_console no_console;  // Не выводит лог следующий за этим флагом в консоль

class TLog : public QObject
{
    Q_OBJECT
public:
    explicit TLog(QString logname);
    ~TLog();

	inline TLog & operator <<(const log_flags::_end & flag) {
		emit add_new_line();
		return *this;
	}

	inline TLog & operator <<(const log_flags::_console & flag) {
		m_console_blocked = true;
		return *this;
	}

	inline TLog & operator <<(const QString & data) {
		print(data);
		return *this;
	}

	inline TLog & operator <<(const std::string & data) {
		print(QString::fromStdString(data));
		return *this;
	}

	inline TLog & operator <<(const char * data) {
		print(QString::fromLocal8Bit(data));
		return *this;
	}

	inline TLog & operator <<(const double data) {
		print(QString::number(data));
		return *this;
	}

signals:
    void data_added(const QString & data);
	void add_new_line();

private:
    bool m_console_blocked;
    QString logName;
    FILE *savefile;

    void append_to_file(const QString & data)
    {
		QString tmp = results_dir +"/"+ result_log+".log";
        savefile = fopen(tmp.toStdString().c_str(), "a");
        if (savefile) {
            fprintf(savefile, "%s\n", data.toStdString().c_str());
            fclose(savefile);
            savefile = NULL;
        }
    }

    inline void print(const QString & data)
    {
        emit data_added(data);
		if (!m_console_blocked) {
            std::cout << data.toStdString() << std::endl;
		}
		if (logName == "fullLog" || logName == "debugLog") {
            append_to_file(data);
		}
        m_console_blocked = false;
    }
};

extern TLog fullLog;
extern TLog debugLog;
extern TLog errorLog;

}

#endif // LOGGER_H

