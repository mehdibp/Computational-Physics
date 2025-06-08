## Molecular Dynamics

In this exercise, we aim to simulate the motion of molecules inside a box.
This time, we will gradually decrease the temperature step by step, allowing the particles to reach thermal equilibrium at each stage.
By doing so, we will transition the gas into a liquid and then convert the liquid into a solid.

The physics of the problem remains exactly the same as explained in the first section, the only new element here is the gradual decrease in temperature.
As the temperature decreases, we expect to see two discontinuities in the energy vs. temperature graph.
These discontinuities indicate phase transitions, marking the **gas-to-liquid** and **liquid-to-solid** transformations.

Initially, I started with a high temperature and waited for $2$ time units to allow the molecules to fully disperse.
Then, I reduced the temperature to $1/16$ of the previous step and continued this process until the temperature reached $0.1$ (**in reduced units**).
If we observe the molecular representation, we expect that as temperature decreases, we will first see scattered triangular networks, indicating the liquid phase.
Later, a larger, more structured triangular network will appear, representing the solid crystalline phase.

<br>

For simulating this structure, we have used the **velocity-Verlet method**, as it ensures that energy remains conserved.

Here, some coefficients are read from a file for convenience, and ultimately, the data is stored in three separate files:
1. One file for the **positions** of each particle over time.
2. Another file for storing **energy** as a function of time, considering energy reduction.
3. A third file for representing the **function $f(t)$**, which shows the average displacement of particles from their initial positions throughout the simulation.

I previously explained the functions in the first exercise, but in this simulation, I made a few modifications:
1. `Update()` function:
    - Adjusted to reduce the temperature by $1/16$ every few steps.
    - Calls `scale()` to adjust velocities according to the new temperature.
    - Waits for $512$ steps to allow particles to reach thermal equilibrium before repeating the process.
2. `init()` function:
    - Defined separate initial positions for both directions, making it easier to track particle displacement from their starting points.
3. `run()` function:
    - Modified to record fewer outputs, preventing the output file size from becoming too large.


<br><br>

The motion of $100$ particles inside a $20×20$ box over a duration of $80$ time units is displayed as follows.

<div align="center">
    <video width="90%" autoplay muted controls loop>
        <source src='./Results/100 particle.mp4' type="video/mp4">
        ./Results/100 particle.mp4
    </video> 
</dev>
<div align="left"></dev><br>

<br>

Based on the video above, we can see that around time $15$, the system transitions into the liquid phase, and approximately at time $50$, it enters the solid phase.

The energy versus temperature graph is also visible below. We expect to observe two discontinuities in the graph at the phase transitions between gas to liquid and liquid to solid. However, this discontinuity is not clearly visible, even though the graph has been plotted on a logarithmic scale!

<div align="center"> <img src='./Results/Ex6-B (temp).png'> </dev>
<div align="left"></dev><br>

The following graphs illustrate the average displacement of particles relative to their initial positions in different states:
1. Average displacement during the simulation
2. Average displacement in the gas phase
3. Average displacement in the solid phase

<br>
<div align="center"> <img src='./Results/Ex6-B (f_t).png'> </dev>
<div align="left"></dev><br>

It is clearly evident that in the gas phase, molecular motion is highly active. However, as soon as the system transitions into the solid phase, the displacement significantly decreases, which is expected.

<br>

Additional Explanation: <br>
The **MATLAB file** (`Particle Display (MD).m`) included in the project is exclusively used to visualize particle motion over time.
This code loads the simulation results stored in the **.txt file**, then displays a real-time snapshot of the particle positions in the system.
