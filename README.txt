How to run
In part1, part2, part3, run make to compile 
sample files that can be used are in the SampleFiles directory
part1 can be run with ./PPScheduler
part2 can be run with ./SRTFScheduler
part3 can be run with ./threadProgram



Sample files
SJF1 - valid sample file for SRTF 
SJF2 - 2nd valid sample file for SRTF
PPnon - valid sample for priority scheduling. All processes arrive at
the same time
PPprempt - valid sample for premptive priority.
wait.txt - valid file, no processes at the begining 
oneLine - valid file consisting of one process
thread - valid file for thread program

Expected output to sample files For SRTF
SJF1.txt:
gantt chart: |P1:0-1|P2:1-4|P1:4-9|P4:9-16|P3:16-24|
average waiting time: 5.75
average turnaround time: 11.75

SJF2.txt:
gantt chart: |P1:0-1|P2:1-5|P4:5-10|P1:10-17|P3:17-26|
average waiting time: 6.5
average turnaround time: 13

oneLine
gantt chart: |none:0-5|P1:5-6|
average waiting time: 0
averate turnaround time: 1

wait.txt
gantt chart: |P1:0-1|none:1-5|P2:5-6|P3:6-7| 
average waiting time: 1.3
average turnaround time: 0.33 


Expected output to sample files For Premptive priority
PPnon
gantt chart: |P2:0-1|P5:1-6|P1:6-16|P3:16-18|P4:18-19|
average waiting time: 8.2
average turnaround time: 12

PPprempt
gantt chart: |P2:0-1|P1:1-4|P5:4-9|P1:9-16|P3:16-18|P4:18-19|
average waiting time: 7.8
average turnaround time: 11.6 

wait.txt
gantt chart: |P1:0-1|none:1-5|P2:5-6|P3:6-7| 
average waiting time: 1.3
average turnaround time: 0.33 

oneLine
gantt chart: |none:0-5|P1:5-6|
average waiting time: 0
averate turnaround time: 1


For threadProgram

Preemptive Priority: gantt: 
|P1: 0-1|P2: 1-2|P5: 2-9|P4: 9-15|P3: 15-20|P2: 20-23|P1: 23-24|
SRTF: gantt: 
|P1: 0-2|P2: 2-6|P3: 6-11|P4: 11-17|P5: 17-24|
SRTF: the average turnaround time = 10.6, the average waiting time = 5.8
PP: the average turnaround time = 16.8, the average waiting time = 12





Expected output for invalid files - "Error: Invalid file"
NotNum































