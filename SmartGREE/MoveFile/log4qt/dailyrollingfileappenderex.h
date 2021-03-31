#ifndef DAILYROLLINGFILEAPPENDEREX_H
#define DAILYROLLINGFILEAPPENDEREX_H

#include "fileappender.h"


#include <QDateTime>

namespace Log4Qt
{

/*!
 * \brief The class DailyRollingFileAppenderEx extends FileAppender so that the
 *        underlying file is rolled over at a specified frequency.
 *
 * \note All the functions declared in this class are thread-safe.
 *
 * \note The ownership and lifetime of objects of this class are managed. See
 *       \ref Ownership "Object ownership" for more details.
 */
class  LOG4QT_EXPORT DailyRollingFileAppenderEx : public FileAppender
{
    Q_OBJECT

    /*!
     * The property holds the date pattern used by the appender.
     *
     * The default is DAILY_ROLLOVER for rollover at midnight each day.
     *
     * \sa fileNamePattern(), setFileNamePattern()
     */
    Q_PROPERTY(QString pathPattern READ pathPattern WRITE setPathPattern)
    Q_PROPERTY(QString fileNamePattern READ fileNamePattern WRITE setFileNamePattern)
    Q_PROPERTY(QString logPath READ logPath WRITE setLogPath)

public:
    /*!
     * The enum DatePattern defines constants for date patterns.
     *
     * \sa setFileNamePattern(DatePattern)
     */
    enum DatePattern
    {
        /*! The minutely date pattern string is "'.'yyyy-MM-dd-hh-mm". */
        MINUTELY_ROLLOVER = 0,
        /*! The hourly date pattern string is "'.'yyyy-MM-dd-hh". */
        HOURLY_ROLLOVER,
        /*! The half-daily date pattern string is "'.'yyyy-MM-dd-a". */
        HALFDAILY_ROLLOVER,
        /*! The daily date pattern string is "'.'yyyy-MM-dd". */
        DAILY_ROLLOVER,
        /*! The weekly date pattern string is "'.'yyyy-ww". */
        WEEKLY_ROLLOVER,
        /*! The monthly date pattern string is "'.'yyyy-MM". */
        MONTHLY_ROLLOVER,
        YEARLY_ROLLOVER, //yyyy
    };
    Q_ENUM(DatePattern)

    DailyRollingFileAppenderEx(QObject *parent = nullptr);
    DailyRollingFileAppenderEx(const LayoutSharedPtr &layout,
                             const QString &fileName,
                             const QString &fileNamePattern,
                               const QString &pathPattern,
                             QObject *parent = nullptr);

    ~DailyRollingFileAppenderEx();

private:
    Q_DISABLE_COPY(DailyRollingFileAppenderEx)
public:

    void activateOptions() override;

    QString fileNamePattern() const
    {
        QMutexLocker locker(&mObjectGuard);
        return mFileNamePattern;
    }

    QString pathPattern() const
    {
        QMutexLocker locker(&mObjectGuard);
        return mPathPattern;
    }

    QString logPath() const
    {
        QMutexLocker locker(&mObjectGuard);
        return m_logPath;
    }

public slots:
    /*!
    * Sets the fileNamePattern to the value specified by the \a fileNamePattern
    * constant.
    */
    void setFileNamePattern(DatePattern fileNamePattern);
    void setPathPattern(DatePattern dateType);

    void setFileNamePattern(const QString &fileNamePattern)
    {
        QMutexLocker locker(&mObjectGuard);
        mFileNamePattern = fileNamePattern;
    }

    void setPathPattern(const QString &pathPattern)
    {
        QMutexLocker locker(&mObjectGuard);
        mPathPattern = pathPattern;
    }

    void setLogPath(const QString& logPath)
    {
        QMutexLocker locker(&mObjectGuard);
        m_logPath = logPath + QStringLiteral("/");
    }

protected:
    void append(const LoggingEvent &event) override;
    bool checkEntryConditions() const override;

private:
    void fileChangeTo(const QString& target_file);

    void computeFrequency();//根据配置计算滚动频率
    void computeRollOvetime();//根据滚动频率计算下一次滚动时间
    QString frequencyToString() const;
    void rollOver();//日期滚动

    void computeFrequencyDir();//根据配置计算滚动频率
    void computeRollOvetimeDir();//根据滚动频率计算下一次滚动时间
    QString frequencyToStringDir() const;
    void rollOverDir();//日期滚动

private:
    QString mFileNamePattern;//日期模式
    DatePattern mFrequency;
    QString mActiveDatePattern;//动态日期模式
    QDateTime mRollOvetime;
    QString mRollOverSuffix;//下一次文件名
    QString mCurrentFile;//当前文件名(包含路径)

    QString mPathPattern;//文件夹日期模式
    DatePattern mFrequencyDir;
    QString mActiveDatePatternDir;//动态文件日期模式
    QDateTime mRollOvetimeDir;
    QString mRollOverSuffixDir;//下一次文件夹名
    QString mCurrentDir;//当前文件夹名

    QString m_logPath;
};


} // namespace Log4Qt


#endif // DAILYROLLINGFILEAPPENDEREX_H
