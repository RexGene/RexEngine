#include "CInterval.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace Util;

const CInterval CInterval::_zero( 0 , 0 );

bool parseIntervalYMD(
        int64_t* time,const char ch,const char* value,
        int index,int length ,int count
        )
{
    char buf[5];
    int nvalue = 0;
    if ( index + count > length )
    {
        return false;
    }
    switch ( ch )
    {
        case 'D' :
            {
                if ( count > 2 )
                    return false;
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi( buf );
                if ( nvalue < 0 )
                    return false;
                *time += nvalue * 24 * 3600 * 1000;
            }
            break;
        case 'h' :
            {
                if ( count > 2 )
                {
                    return false;
                }
                memcpy(buf, value + index, count);
                buf[count] = 0;
                nvalue = atoi( buf );
                if ( nvalue >= 24 || nvalue < 0 )
                    return false;
                *time += nvalue * 3600 * 1000;
            }
            break;
        case 'm' :
            {
                if ( count > 2 )
                {
                    return false;
                }
                memcpy(buf,value+index,count);
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue >= 60 || nvalue < 0 )
                    return false;
                *time += nvalue * 60 * 1000 ;
            }
            break;
        case 's' :
            {
                if ( count > 2 )
                {
                    return false;
                }
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi( buf );
                if ( nvalue >= 60 || nvalue < 0 )
                {
                    return false;
                }
                *time += nvalue * 1000 ;
            }
            break;
        case 'n' :
            {
                if ( count > 3 )
                {
                    return false;
                }
                memcpy( buf, value + index, count );
                buf[count] = 0;
                nvalue = atoi(buf);
                if ( nvalue < 0 )
                {
                    return false;
                }
                *time += nvalue;
            }
            break;
        default :
            return false;
    }
    return true;
}

CInterval::CInterval(int days , int hours, int minutes, int seconds, int millSeconds )
{
    this->_timeSpan = millSeconds;
    this->_timeSpan += int64_t(seconds) * 1000;
    this->_timeSpan += int64_t(minutes) * 60 * 1000;
    this->_timeSpan += int64_t(hours) * 60 * 60 * 1000;
    this->_timeSpan += int64_t(days) * 24 * 60 * 60 * 1000;
}

CInterval::CInterval(const CInterval& other)
{
    _timeSpan = other._timeSpan;
}

CInterval::CInterval( const int64_t millSeconds )
{
    _timeSpan = millSeconds;
}
CInterval::CInterval( const int second , const int micro )
{
    _timeSpan = second * 1000;
    _timeSpan += micro / 1000;
}

CInterval::CInterval() : _timeSpan(0)
{
}

int CInterval::getDays() const
{
    return (long)(_timeSpan / (24 * 60 * 60 * 1000));
}

int CInterval::getTotalSeconds() const
{
    return (long)(_timeSpan / 1000);
}

int CInterval::getTotalMinutes() const
{
    return (long)(_timeSpan / (60 * 1000));
}

int CInterval::getTotalHours() const
{
    return (long)(_timeSpan / (60 * 60 * 1000));
}

int64_t CInterval::getTotalMills() const
{
    return _timeSpan;
}

int CInterval::getSecondsOfDay() const
{
    return getTotalSeconds() - getTotalMinutes() * 60 ;
}

int CInterval::getMinutesOfDay() const
{
    return getTotalMinutes() - getTotalHours() * 60 ;
}

int CInterval::getHoursOfDay() const
{
    return getTotalHours() - getDays() * 24 ;
}

int CInterval::getMillsOfDay() const
{
    return (long)(_timeSpan - getTotalSeconds() * 1000);
}

CInterval CInterval::operator -(const CInterval& other) const
{
    CInterval result(*this);
    result -= other;
    return result;
}

CInterval CInterval::operator +(const CInterval& other) const
{
    CInterval result(*this);
    result += other;
    return result;
}

const CInterval& CInterval::operator -=(const CInterval& other)
{
    this->_timeSpan -= other._timeSpan;
    return *this;
}

const CInterval& CInterval::operator +=(const CInterval& other)
{
    this->_timeSpan += other._timeSpan;
    return *this;
}

bool CInterval::operator ==(const CInterval& other) const
{
    return this->_timeSpan == other._timeSpan;
}

bool CInterval::operator !=(const CInterval& other) const
{
    return this->_timeSpan != other._timeSpan;
}

bool CInterval::operator <(const CInterval& other) const
{
    return this->_timeSpan < other._timeSpan;
}

bool CInterval::operator <=(const CInterval& other) const
{
    return this->_timeSpan <= other._timeSpan;
}

bool CInterval::operator >(const CInterval& other) const
{
    return this->_timeSpan > other._timeSpan;
}

bool CInterval::operator >=(const CInterval& other) const
{
    return this->_timeSpan >= other._timeSpan;
}

bool CInterval::parse(const char* value,const char* format)
{
    _timeSpan = 0;
    int len = (int)strlen(format);
    int vallen = (int)strlen(format);
    int index = 0;
    for ( index = 0 ; index < len ; )
    {
        char ch = format[index];
        switch ( ch )
        {
            case 'D' :
            case 'h' :
            case 'm' :
            case 's' :
            case 'n' :
                {
                    int count = 1;
                    while ( format[++index] == ch )
                        count++;

                    if ( !parseIntervalYMD(&_timeSpan, ch, value , index-count, vallen, count ) )
                    {
                        return false;
                    }
                };
                break;
            default :
                index ++;
        }
    }

    return true;
}

std::string CInterval::asString(const char* format) const
{
    char buf[100];
    sprintf(buf,"%s",format);
    int len = (int)strlen(format);
    int index = 0;
    for ( index = 0 ; index < len ; )
    {
        char ch = format[index];
        switch ( ch )
        {
            case 'D' :
            case 'h' :
            case 'm' :
            case 's' :
            case 'n' :
                {
                    int count = 1;
                    while ( format[++index] == ch )
                    {
                        count++;
                    }
                    if ( !asYMD( ch , buf , index - count , count , sizeof(buf) ) )
                    {
                        return std::string(NULL);
                    }
                }
                break;
            default :
                index ++;
        }
    }
    return std::string(buf);
}

bool CInterval::asYMD(const char ch, char* buf, int index, int count, int bufSize ) const
{
    if ( index + count >= bufSize )
        return false;
    char change;
    char format[5];
    sprintf(format,"%%0%dd",count);
    change = buf[index + count];
    switch ( ch )
    {
        case 'D' :
            if ( count > 2 )
                return false;
            sprintf( buf + index , format , this->getDays() );
            break;
        case 'h' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, this->getHoursOfDay() );
            break;
        case 'm' :
            if ( count > 2 )
                return false;
            sprintf( buf + index , format , this->getMinutesOfDay() );
            break;
        case 's' :
            if ( count > 2 )
                return false;
            sprintf( buf + index, format, this->getSecondsOfDay() );
            break;
        case 'n' :
            if ( count > 3 )
                return false;
            sprintf( buf + index , format , this->getMillsOfDay() );
            break;
        default :
            return false;
    }
    buf[index + count] = change;
    return true;
}

