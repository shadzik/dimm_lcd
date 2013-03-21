/* Dimm_lcd by Bartosz ¦wi±tek aka shadzik (shadzik@atwa.us)
 * Let's you easily dimm your LCD on Toshiba notebooks
 * ADVICE: use with FnFx as Fn-F2 key shortcut
 * version: 0.4, 2005.12.06
 * Thanks to kflis
 *
 * g++ -O3 -o dimm_lcd dimm_lcd2.cpp
 *
 * ChangeLog:
 * version 0.4 2005.12.06:
 * - added cpufreq();
 * version 0.3 2005.06.21:
 * - ios::app -> ios::out
 * version 0.2:
 * - added smooth-like effect
 * version 0.1:
 * - initial version
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>

#define DIMM "/proc/acpi/toshiba/lcd"
#define GOVERNOR "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor"


using namespace std;

/*
 * Check the video status. This is probably the most important function.
 */
int video_status()
{
   string somestr, parameter;
   ifstream plik (DIMM);
   if (!plik.is_open())
   {
      cout << "File read error.\n";
      exit (1);
   }

   if(!plik.eof())
   {
      getline(plik, somestr);
      size_t pos = somestr.find_first_of(" ");
      parameter.resize(somestr.size() - pos);
      copy(somestr.begin() + pos, somestr.end(), parameter.begin());
      stringstream sstr;
      int real_value;
      sstr << parameter;
      sstr >> real_value;
      //cout << real_value << endl;
      return real_value;
   }   
}
/*
 * uncomment to use it
int cpufreq(bool save)
{
   ofstream plik (GOVERNOR, ios::out);
   if (plik.is_open())
   {
      if (save)
      {
         ofstream plik (GOVERNOR, ios::out);
         plik << "powersave";
         plik.close();
      }
      else
      {
         ofstream plik (GOVERNOR, ios::out);
         plik << "performance";
         plik.close();
      }
   } else {
      cout << "File open error.\n";
   }
}

*/

int wpis()
{
   int value;
   ofstream plik (DIMM, ios::out);
   value = video_status();
   if (plik.is_open())
   {
      plik.close();
      if (value < 7)
      {
         for (int i=value; i<=7; i++) //to get a smooth-like effect
         {
             ofstream plik (DIMM, ios::out);
             plik << "brightness:" << i;
             plik.close();
         }
	 //cpufreq(false);
      }
      if (value == 7)
      {
         for (int i=value; i>=0; i--) //the same effect
         {             
             ofstream plik (DIMM, ios::out);
             plik << "brightness:" << i;
             plik.close();
         }
	 //cpufreq(true);
      }
      //plik.close();
   } else {
      cout << "File open error.\n";
   }
}

main()
{
   wpis();
}
