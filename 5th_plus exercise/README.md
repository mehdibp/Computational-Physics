## Solar System 

In this exercise, we aim to simulate the approximate motion of Earth in the solar system. To simplify the model, we only consider the forces exerted by the Sun and Jupiter on Earth while ignoring other planets and celestial bodies due to their negligible effects

<div> <img src='./free-body diagram.png' width=75%> </dev>

Here, we consider the mass of Earth as $m$ and the mass of Jupiter as $M_{j}$, and we name the forces acting on each accordingly.
Thus, the forces can be expressed as follows.

$F_{je_{x}}=  \frac{GM_{j} m}{r_{ej}^2 } \cos⁡(\varphi)= - \frac{GM_{j} m}{r_{ej}^2} \frac{x-X}{r_{ej}^2}$ 

$F_{je_{y}} =  \frac{GM_{j} m}{r_{ej}^2 } \sin⁡(\varphi)= - \frac{GM_{j} m}{r_{ej}^2} \frac{y-Y}{r_{ej}^2}$

Due to Earth's low mass, we can neglect its gravitational effect on Jupiter, setting $F_{ej} = 0$.
The two remaining forces, namely the Sun's gravitational force on the planets, can be easily determined using Newton's law of gravitation.
Thus, Jupiter's equations of motion are as follows.

$\frac{dV_{x}}{dt}= -\frac{GM_{s} X}{R^3}+O(m)$

$\frac{dV_{y}}{dt}= -\frac{GM_{s} Y}{R^3}+O(m)$

And the equations of motion for Earth will be as follows.

$\frac{dv_{x}}{dt}= -\frac{GM_s x}{r^3} - \frac{GM_{j} (x-X)}{r_{ej}^3}$

$\frac{dv_{y}}{dt}= -\frac{GM_s y}{r^3} - \frac{GM_{j} (y-Y)}{r_{ej}^3}$

In these equations,

* $R = \sqrt{X^2+Y^2}$
* $r = \sqrt{x^2+y^2}$
* $r_{ej} = \sqrt{(x-X)^2+(y-Y)^2}$

To simplify the calculations, we use the reduced unit system, where the unit of length is one astronomical unit **AU** and the unit of time is one year.
Thus, the final equations in the reduced unit system will be as follows

* $\frac{dV_{x}}{dt}= -4π^2 \frac{X}{R^3}$
* $\frac{dV_{y}}{dt}= -4π^2 \frac{Y}{R^3}$

<br>

* $\frac{dv_{x}}{dt}= -4π^2 \frac{x}{r^3} - \frac{0.001 (x-X)}{r_{ej}^3}$ 
* $\frac{dv_{y}}{dt}= -4π^2 \frac{y}{r^3} - \frac{0.001 (y-Y)}{r_{ej}^3}$

Where $0.001$ is obtained from the division of Jupiter's mass by the Sun's mass.

<br>

Based on the obtained equations, we write the program using the **Euler-Cromer** method.
In this program, for simplicity and ease of modification, some data is read from a file. After performing mathematical operations in the **reduced unit** system, the final values are stored separately in the **SI unit** system in another file


<br>

The following figure, which is the first obtained result, shows the orbits of Earth and Jupiter in both the reduced unit system and the SI system.
Based on the shape of the orbits, it can be seen that their paths are very close to circular. (which is expected from the actual system)

<div align="center"> <img src='./Ex5+ -1.png' width=90%> </dev>
<div align="left"></dev><br>

As observed, both orbits (Earth: blue, Jupiter: orange) remain stable, with no deviation in their motion over time.
This means that, just like in the real system, both planets are continuously revolving in their designated orbits.

<br>

For the second part of the question, we were asked to increase Jupiter's mass so that, in the end, the behavior of Jupiter and the Sun would resemble that of a binary star system (i.e., having an equal mass ratio).
To achieve this, we gradually adjusted Jupiter’s mass ratio relative to the Sun in three steps, setting its value to $0.01$, $0.1$, and $1$, respectively.
Observing Earth's orbital behavior over time in this scenario should be interesting. Therefore, we produced the following results based on the given descriptions.


<div align="center"> <img src='./Ex5+ -2.png'> </dev>
<div align="left"></dev><br>

As observed, with the increase in Jupiter's mass, Earth's orbit (blue graph) becomes progressively more unstable.
Ultimately, the instability reaches a level where, within just a quarter of Jupiter's orbit, Earth completely exits the solar system.


