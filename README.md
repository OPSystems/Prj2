# Prj2: Exercises in Concurrency and Synchronization

## - Harshil Kotamreddy, Tim Ellis, Lynne Tien

**Objectives**
<p>Learn to:<br>
- Solve inter-process communication problems during concurrent execution of processes.<br>
- Use Posix Pthread library for concurrency.</p>
<br>
<p> Instructions To Run Each Program:<br>

1. Bounded-buffer Producer/Consumer problem:<br>
    - Open terminal
    - <code>cd</code> into the directory where the file resides
    - At the command prompt, type <code>gcc procon.c -o procon -lpthread</code> then press enter
    - Then type <code>./procon</code> and again press enter
    - Then program will run for infinity so user must press 'cntrl c' to exit
    <br>
    
2. Mother Hubbard (children, mother, father)
    - Open terminal
    - <code>cd</code> into the directory where the file resides
    - At the command prompt, type <code>gcc mh.c -o mh -lpthread</code> then press enter
    - Then type: <code>./mh `<number of days>`</code> and again press enter. For example typing <code>./mh 2</code> will launch the program for 2 days.
    <br>
    
3. Airline Passengers
    - Open terminal
    - <code>cd</code> into the directory where the file resides
    - At the command prompt, type <code>gcc airline.c -o airline -lpthread</code> then press enter
    - Then type <code>./airline `<number of passengers>` `<number of baggage handlers>` `<number of security screeners>` `<number of flight attendants>`</code> and again press enter. For example typing <code>./airline 100 3 5 2</code> will launch the program with 100 passengers, 3 baggage handlers, 5 security screeners, 2 flight attendants.
    <br>