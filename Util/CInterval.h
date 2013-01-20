#pragma once

#include <stdint.h>
#include <time.h>
#include <string>

namespace Util
{
    const char DEFAULT_INTERVAL_FORMAT[] = "DD hh:mm:ss.nnn";

    class CInterval
    {
        public:
            const static CInterval _zero;

        private:
            friend class CDateTime;
            int64_t _timeSpan;

        private:
            bool asYMD(const char ch,char* buf,int index,int count,int bufsize) const;

        public:

            CInterval(int days, int hours, int minutes, int seconds,int millseconds);

            CInterval(const CInterval& other);

            CInterval( const int second , const int micro );

            CInterval(const int64_t millseconds);

            CInterval();

            int getDays() const;

            int getTotalSeconds() const;

            int getTotalMinutes() const;

            int getTotalHours() const;
            int64_t getTotalMills() const;

            int getSecondsOfDay() const;

            int getMinutesOfDay() const;

            int getHoursOfDay() const;

            int getMillsOfDay() const;

            CInterval operator -(const CInterval& other) const;

            CInterval operator +(const CInterval& other) const;

            const CInterval& operator -=(const CInterval& other);

            const CInterval& operator +=(const CInterval& other);

            bool operator ==(const CInterval& other) const;

            bool operator !=(const CInterval& other) const;

            bool operator <(const CInterval& other) const;

            bool operator <=(const CInterval& other) const;

            bool operator >(const CInterval& other) const;

            bool operator >=(const CInterval& other) const;

            bool parse(const char* value,const char* format = DEFAULT_INTERVAL_FORMAT);

            std::string asString(const char* format = DEFAULT_INTERVAL_FORMAT) const;

    };

}
