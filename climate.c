/**
 * climate.c
 *
 * Performs analysis on climate data provided by the
 * National Oceanic and Atmospheric Administration (NOAA).
 *
 * Input:    Tab-delimited file(s) to analyze.
 * Output:   Summary information about the data.
 *
 * Compile:  run make
 *
 * Example Run:      ./climate data_tn.tdv data_wa.tdv
 *
 *
 * Opening file: data_tn.tdv
 * Opening file: data_wa.tdv
 * States found: TN WA
 * -- State: TN --
 * Number of Records: 17097
 * Average Humidity: 49.4%
 * Average Temperature: 58.3F
 * Max Temperature: 110.4F 
 * Max Temperatuer on: Mon Aug  3 11:00:00 2015
 * Min Temperature: -11.1F
 * Min Temperature on: Fri Feb 20 04:00:00 2015
 * Lightning Strikes: 781
 * Records with Snow Cover: 107
 * Average Cloud Cover: 53.0%
 * -- State: WA --
 * Number of Records: 48357
 * Average Humidity: 61.3%
 * Average Temperature: 52.9F
 * Max Temperature: 125.7F
 * Max Temperature on: Sun Jun 28 17:00:00 2015
 * Min Temperature: -18.7F 
 * Min Temperature on: Wed Dec 30 04:00:00 2015
 * Lightning Strikes: 1190
 * Records with Snow Cover: 1383
 * Average Cloud Cover: 54.5%
 *
 * TDV format:
 *
 * CA» 1428300000000»  9prcjqk3yc80»   93.0»   0.0»100.0»  0.0»95644.0»277.58716
 * CA» 1430308800000»  9prc9sgwvw80»   4.0»0.0»100.0»  0.0»99226.0»282.63037
 * CA» 1428559200000»  9prrremmdqxb»   61.0»   0.0»0.0»0.0»102112.0»   285.07513
 * CA» 1428192000000»  9prkzkcdypgz»   57.0»   0.0»100.0»  0.0»101765.0» 285.21332
 * CA» 1428170400000»  9prdd41tbzeb»   73.0»   0.0»22.0»   0.0»102074.0» 285.10425
 * CA» 1429768800000»  9pr60tz83r2p»   38.0»   0.0»0.0»0.0»101679.0»   283.9342
 * CA» 1428127200000»  9prj93myxe80»   98.0»   0.0»100.0»  0.0»102343.0» 285.75
 * CA» 1428408000000»  9pr49b49zs7z»   93.0»   0.0»100.0»  0.0»100645.0» 285.82413
 *
 * Each field is separated by a tab character \t and ends with a newline \n.
 *
 * Fields:
 *      state code (e.g., CA, TX, etc),
 *      timestamp (time of observation as a UNIX timestamp),
 *      geolocation (geohash string),
 *      humidity (0 - 100%),
 *      snow (1 = snow present, 0 = no snow),
 *      cloud cover (0 - 100%),
 *      lightning strikes (1 = lightning strike, 0 = no lightning),
 *      pressure (Pa),
 *      surface temperature (Kelvin)
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES 50

/* TODO: Add elements to the climate_info struct as necessary. */
struct climate_info {
  unsigned long num_records;
    char code[3];
    double sum_humidity;
    unsigned long snow;
    long double sum_cloudcover;
    unsigned long lightning;
    long double sum_pressure;
    long double sum_temperature;
    double max_temp;
    char max_time[15];
    double min_temp;
    char min_time[15];
};

void analyze_file(FILE *file, struct climate_info *states[], int num_states);
void print_report(struct climate_info *states[], int num_states);

int main(int argc, char *argv[]) {

    /* TODO: fix this conditional. You should be able to read multiple files. */
    // if (argc != 2) {
    //     printf("Usage: %s tdv_file1 tdv_file2 ... tdv_fileN \n", argv[0]);
    //     return EXIT_FAILURE;
    // }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 US states. */
    struct climate_info *states[NUM_STATES] = { NULL };
    int j;
    for(int j=0;j<NUM_STATES;j++){
      *(states+j)=malloc(sizeof(struct climate_info));
    }
    int num_states=NUM_STATES;
    int i;
    FILE* fp;
    for (i = 1; i < argc; ++i) {
        /* TODO: Open the file for reading */

        /* TODO: If the file doesn't exist, print an error message and move on
         * to the next file. */

        /* TODO: Analyze the file */
        /* analyze_file(file, states, NUM_STATES); */
      fp=fopen(argv[i],"r");
      if (fp == NULL) {
        printf("Error in opening file %s\n",argv[i]);
        continue;
      }
      analyze_file(fp, states, num_states);
    }
    /* Now that we have recorded data for each file, we'll summarize them: */
    print_report(states, NUM_STATES);
    return 0;
}

void analyze_file(FILE *file, struct climate_info **states, int num_states) {
    const int line_sz = 100;
    char line[line_sz];
    while (fgets(line, line_sz, file) != NULL) {
        char delim[2]="\t";
        char* code=strtok(line, delim);
        char* time=strtok(NULL,delim);
        char* location=strtok(NULL,delim);
        char* humidity=strtok(NULL,delim);
        char* snow=strtok(NULL,delim);
        char* cloud=strtok(NULL,delim);
        char* lightning=strtok(NULL,delim);
        char* pressure=strtok(NULL,delim);
        char* temperature=strtok(NULL,delim);

        double dtemp=strtod(temperature,NULL)*1.8-459.67;

        int i;
        int already_exist=0;
        struct climate_info* current;
        for(i=0;i<num_states;i++){
          if((*(states+i))->num_records==0){
            break;
          }
          char* curr_code=(*(states+i))->code;
          if(strcmp(curr_code, code)==0){
            current=*(states+i);
            already_exist=1;
            break;
          }
          
        }
        if(!already_exist){
          current=*(states+i);
          current->num_records=1;
          strcpy(current->code, code);
          current->sum_humidity=strtold(humidity,NULL);
          current->snow=strtoul(snow,NULL,10);
          current->sum_cloudcover=strtold(cloud,NULL);
          current->lightning=strtoul(lightning,NULL,10);
          current->sum_pressure=strtoul(pressure,NULL,10);
          current->sum_temperature=dtemp;
          current->max_temp=dtemp;
          current->min_temp=dtemp;
          strcpy(current->max_time, time);
          strcpy(current->min_time, time);
        }else{
          current->num_records++;
          current->sum_humidity+=strtold(humidity,NULL);
          current->snow+=strtoul(snow,NULL,10);
          current->sum_cloudcover+=strtold(cloud,NULL);
          current->lightning+=strtoul(lightning,NULL,10);
          current->sum_pressure+=strtoul(pressure,NULL,10);
          current->sum_temperature+=dtemp;
          if(dtemp>current->max_temp){
            current->max_temp=dtemp;
            strcpy(current->max_time, time);
          }
          if(dtemp<current->min_temp){
            current->min_temp=dtemp;
            strcpy(current->min_time, time);
          }
      
        }
        /* TODO: We need to do a few things here:
         *
         *       * Tokenize the line.
         *       * Determine what state the line is for. This will be the state
         *         code, stored as our first token.
         *       * If our states array doesn't have a climate_info entry for
         *         this state, then we need to allocate memory for it and put it
         *         in the next open place in the array. Otherwise, we reuse the
         *         existing entry.
         *       * Update the climate_info structure as necessary.
         */

    }
}

void print_report(struct climate_info *states[], int num_states) {
    printf("States found:\n");
    int i;
    for (i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            struct climate_info *info = states[i];
            printf("%s ", info->code);
        }
    }
    printf("\n");
      /* TODO: Print out the summary for each state. See format above. */

    for(i=0;i<num_states;i++){
      if (states[i]->num_records != 0) {
            struct climate_info *info = states[i];
            unsigned long num_records=info->num_records;
            double average_humidity=info->sum_humidity/num_records;
            double average_temperature=info->sum_temperature/num_records;
            double max_temperature=info->max_temp;
            char* max_time=info->max_time;
            double min_temperature=info->min_temp;
            char* min_time=info->min_time;
            unsigned long lightenings=info->lightning;
            unsigned long snow=info->snow;
            double average_cloud=info->sum_cloudcover/num_records;

            printf("-- State: %s --\n", info->code);
            printf("Number of Records: %lu\n",num_records);
            printf("Average Humidity: %0.1f%%\n",average_humidity);
            printf("Average Temperature: %0.1fF\n",average_temperature);
            printf("Max Temperature: %0.1fF\n",max_temperature);
            time_t max_t = (time_t) strtoll(max_time, NULL, 10)/1000; 
            printf("Max Temperature on: %s",ctime(&(max_t)));
            printf("Min Temperature: %0.1fF\n",min_temperature);
            time_t min_t = (time_t) strtoll(min_time, NULL, 10)/1000; 
            printf("Min Temperature on: %s\n",ctime(&(min_t)));
            printf("Lightning Strikes: %lu\n", lightenings);
            printf("Records with Snow Cover: %lu\n", snow);
            printf("Average Cloud Cover: %0.1f%%\n", average_cloud);
        }
    }
}
