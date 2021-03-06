#pragma once
#include "Types.h"

namespace SE3D2
{
	struct DateTime
	{
		int32 Hour;
		int32 Minute;
		int32 Seconds;
		int32 Day;
		int32 Month;
		int32 Year;

		inline bool IsValid()
		{
			if (*this == DateTime{ 0,0,0,0,0,0 })
			{
				return false;
			}
			return true;
		}

		bool operator<(const DateTime& date) const
		{
			if (Year > date.Year) { return false; }
			else if (Year < date.Year) { return true; }
			if (Month > date.Month) { return false; }
			else if (Month < date.Month) { return true; }
			if (Day > date.Day) { return false; }
			else if (Day < date.Day) { return true; }
			if (Hour > date.Hour) { return false; }
			else if (Hour < date.Hour) { return true; }
			if (Minute > date.Minute) { return false; }
			else if (Minute < date.Minute) { return true; }
			if (Seconds > date.Seconds) { return false; }
			else if (Seconds < date.Seconds) { return true; }
			return true;
		}

		bool operator>(const DateTime& date) const
		{
			if (Year < date.Year) { return false; }
			else if (Year > date.Year) { return true; }
			if (Month < date.Month) { return false; }
			else if (Month > date.Month) { return true; }
			if (Day < date.Day) { return false; }
			else if (Day > date.Day) { return true; }
			if (Hour < date.Hour) { return false; }
			else if (Hour > date.Hour) { return true; }
			if (Minute < date.Minute) { return false; }
			else if (Minute > date.Minute) { return true; }
			if (Seconds < date.Seconds) { return false; }
			else if (Seconds > date.Seconds) { return true; }
			return true;
		}

		bool operator>=(const DateTime& date) const
		{
			return !(*this < date);
		}

		bool operator<=(const DateTime& date) const
		{
			return !(*this > date);
		}

		bool operator==(const DateTime& date) const
		{
			if (Seconds != date.Seconds) { return false; }
			if (Minute != date.Minute) { return false; }
			if (Hour != date.Hour) { return false; }
			if (Day != date.Day) { return false; }
			if (Month != date.Month) { return false; }
			if (Year != date.Year) { return false; }
			return true;
		}

		bool operator!=(const DateTime& date) const
		{
			return !(*this == date);
		}

	};
}