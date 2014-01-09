.. $Id$

=================================================
Example: Feedback using the ``epid`` record
=================================================

.. note: this page is under construction

This example builds on the previous example where a sensor 
creating a *signal* was connected to the Arduino and monitored 
from EPICS.  An additional sensor was connected to the Arduino to 
*control* (or modify) the first signal.

Requirements:

* Arduino system with specified electronics circuit [#]_
* Linux computer with Arduino interface
* EPICS IOC configured to communicate with Arduino
* CSS BOY client for EPICS [#]_

The EPICS *epid* record [#epid]_ is used to continuously update the 
*control* based on updates to the *signal* with the goal of 
driving the signal to a desired *set point*.  The *epid* record 
provides an extended Proportional-Integral-Derivative controller 
(see, for example, these documents from the University of 
Michigan: [#]_, [#]_, [#UMich]_) to a process configured by EPICS PVs. 
Additional equations are sketched out [#]_ that convert the terms 
of the UMich documentation into terms of the *epid* record.


Basic PID Theory
==================

At its heart, PID control is implemented to maintain a time-dependent, 
measured signal, :math:`M(t)`, at a desired value, :math:`D(t)` 
by adjusting a control, :math:`Y(t)`.  The general PID equation is based
on the concept of a *process error*, :math:`\epsilon(t)`, that is the 
difference between the actual and desired values of the signal:

.. math::

   \epsilon(t) = M(t) - D(t)

general PID equation ([#UMich]_):

.. math::

  Y(t) = K_c \left[ { \epsilon(t) + {1 \over T_i}\int_0^t \epsilon(t_i)d{t_i} + T_d {d\epsilon(t) \over dt} } \right] + Y(t_0)

*epid* record PID equation ([#epid]_):

.. math::

  Y_n = K_P \epsilon(t) + K_P K_I \sum_i {\epsilon_i \delta t_i} + K_P K_D {\epsilon_n - \epsilon_{n-1} \over \delta t_n}

Outline
============

* define the example
* describe the variables
* describe the *epid* configuration (starting with default)
* set an initial value of :math:`K_p`
* measure the system response

Definition of Terms
=====================

====================== =================================================================
term                   description
====================== =================================================================
:math:`t`              time
:math:`M(t)`           measured input **signal** as a function of time
:math:`D(t)`           **set point** (desired value of **signal**) as a function of time
:math:`Y(t)`           chosen **control** output as a function of time
:math:`\epsilon(t)`    **following error**: :math:`\epsilon(t) = M(t) - D(t)`
:math:`K`              generalized process **gain**:  :math:`K = {\hbox{change in output} \over \hbox{change in input}}`
:math:`K_c`            process **gain** constant (from theory, to be determined empirically)
:math:`T_i`            process integral coefficient (from theory, to be determined empirically)
:math:`T_d`            process derivative coefficient (from theory, to be determined empirically)
:math:`K_P`            proportional gain coefficient (EPICS user input):  :math:`K_P = K_C`
:math:`K_I`            integral gain coefficient (EPICS user input):  :math:`K_I = {K_C \over T_i}`
:math:`K_D`            derivative gain coefficient (EPICS user input):  :math:`K_D = K_C T_d`
:math:`\tau`           time for response to complete
:math:`\tau_d`         dead time before system starts to respond
====================== =================================================================


References
==========

.. [#] refer to the circuit design
.. [#] CSS BOY, http://ics-web.sns.ornl.gov/css/products.html
.. [#epid] http://cars9.uchicago.edu/software/epics/epidRecord.html
.. [#] https://controls.engin.umich.edu/wiki/index.php/Main_Page#PID_control
.. [#] https://controls.engin.umich.edu/wiki/index.php/Main_Page
.. [#UMich] https://controls.engin.umich.edu/wiki/index.php/PIDTuningClassical 
.. [#] notes: :download:`131108115836_0001.pdf`
