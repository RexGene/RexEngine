#pragma once

#include "CLock.h"
#include "CInterval.h"

#include <stdint.h>
#include <time.h>
#include <string>

namespace Util
{

    const char DEFAULT_DATETIME_FORMAT[] = "YYYY-MM-DD hh:mm:ss.nnn";
    const std::string NORMAL_DATETIME_FORMAT = "YYYY-MM-DD hh:mm:ss";

    class CDateTime
    {
        private:
            short _timeZone;

            short _showZone;

            int64_t _timeSpan;

            //time_t _time;
            tm* _tm;

        public:
            static CDateTime _lastDt;
            static int _localTimeZone;

        private :
            long getTimeZoneMills() const;

            bool asYMD(struct tm* time,unsigned long mills,
                    const char ch,char* buf,int index,
                    int count,int bufsize) const;

        public:
            void update();

            bool init(
                    int year, int month, int day, int hour,
                    int minute, int second,int millseconds,
                    short timezone = 0);

            bool init( int64_t millseconds , short timezone = 0 );

        public:

            std::string asString(const char* format = DEFAULT_DATETIME_FORMAT) const;

            CDateTime();

            CDateTime(int year, int month, int day, int hour,
                    int minute, int second,int millSeconds,
                    short timezone = 0);

            CDateTime( int64_t millseconds , short timezone = 0 );

            CDateTime(const CDateTime& other);

            virtual ~CDateTime();

            int getYear() const;

            int getMonth() const;

            int getDay() const;

            int getHour() const;

            int getMinute() const;

            int getSecond() const;

            int getDayOfWeek() const;

            int getMillSecond() const;

            void clearMillSecond();

            int getTotalDay() const;

            int64_t getTotalMill() const;

            long getTotalSecond() const;

            CInterval operator -(const CDateTime& other) const;

            CDateTime operator -(const CInterval& interval) const;

            CDateTime operator +(const CInterval& interval) const;

            CDateTime& operator =(const CDateTime& other);

            const CDateTime& operator -=(const CInterval& interval);

            const CDateTime& operator +=(const CInterval& interval);

            bool operator==(const CDateTime& other) const;

            bool operator !=(const CDateTime& other) const;

            bool operator <=(const CDateTime& other) const;

            bool operator <(const CDateTime& other) const;

            bool operator >(const CDateTime& other) const;

            bool operator >=(const CDateTime& other) const;

            bool parse(const char* value, const char* format = DEFAULT_DATETIME_FORMAT);

            void setShowTimeZone(short timezone);

        public:
            static CDateTime getAbsDt();
            static CLock* getLock();
            static void updateThreadDt();
            static CDateTime getThreadDt();

            static void setLocalTimeZone( int localTimeZone );

        private:
            const tm* getLocalTime() const;

            void clearTm();

    };

}

