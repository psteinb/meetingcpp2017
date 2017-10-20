---
title: The Performance Addict's Toolbox
author: Peter Steinbach
origin: Scionics Computer Innovation GmbH
email: steinbach@scionics.de
date: Nov 11, 2017, MeetingC++ 2017, Berlin
---


## Agenda 


1. `whoami` and Motivation
2. Performance outside-in
3. Performance inside-out
4. Benchmarks and how to create them


# whoami and Motivation

## Disclaimer

.container-fluid[

.row justify-content-center[

  .col[

![](img/opensource-550x475.png){ class="figure-img img-fluid" width="40%" }  

**[github.com/psteinb/meetingcpp2017](https://github.com/psteinb/meetingcpp2017)**


  .]

.]

.]


:notes[

report bugs and questions there!

:]


## `whoami`

.container-fluid[

.row align-items-center[

.col[

![](img/events_header_m_quarter.jpg){ class="figure-img img-fluid" width="50%" }

.]

.col[

**[Scionics Computer Innovation GmbH](www.scionics.de)**

- software and consulting company
- founded in 2001 in Dresden, Germany
- expertise in data analysis, bioinformatics, image analysis, HPC, ...

.]

.]

.]


:notes[

- 2h by car south of Berlin  
- NEXT: biggest client = CBG

:]



## Our Client


.container-fluid[

.row align-items-center[

.col[

![MPI for Molecular Cell Biology and Genetics](img/800px-MPI-CBG_building_outside_4pl.jpg){ class="figure-img img-fluid" width="100%" }  

[mpi-cbg.de](www.mpi-cbg.de)

.]

.col[

- 500+ staff
- my role: *Scientific Software Engineer*
- support users on our HPC infrastructure
- software projects related to performance (think multi-threaded, GPUs, ..)

.]

.]

.]


:notes[

- biggest client

:]


# Performance Outside-In

:notes[

- application running in a shell (aka the server)

:]


## One day as a Performance Engineer

![Alan O'Rourke, [Too Busy To Improve - Performance Management - Square Wheels](https://www.flickr.com/photos/toddle_email_newsletters/15412982829/in/photostream/), CC](img/flickr_ORourke_2_busy_to_improve_1200px.png){ class="figure-img img-fluid" width="70%" }

:notes[

- scientists typically develop algorithms (to publish)
- performance is important for usablity

:]

## Once in a while { data-background-image="img/frustration-cry-1682140_1920.jpg" }

.container[

.row align-items-center[

.col[

```
From: doe@theinstitute.de
Subject: Cluster is slow
Date: Fri, 20 Oct 2017 12:03:21 +0200
To: hpcsupport@theinstitute.de

Hi,

what is going on with the cluster? My application is running
slow since yesterday. 
Could you have a look at it please?

Thanks,
John
```

.]

.]

.]


:notes[

- speed is a subjective measure
- performance is a matter of perspective
- note: description for reproducibility missing
- https://youtu.be/FnGCDLhaxKU?t=6152

:]



## High Level Overview


.container-fluid[

.row align-items-center[

.col[

![[htop](http://hisham.hm/htop/), [free](https://linux.die.net/man/1/free) et al](img/htop_in_action.png){ class="figure-img img-fluid" width="50%" }  

.]

.col[

![Task Manager, [Image](https://en.wikipedia.org/wiki/File:Win7-tskman-perf.png) courtesy of Fergie4000, CC-BY 3.0](img/Win7-tskman-perf.png)){ class="figure-img img-fluid" width="50%" }  

.]

.]

.]
