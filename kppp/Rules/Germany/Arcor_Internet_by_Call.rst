#################################################################################
# German Mannesmann Arcor - Internet by Call accounting rules
#
# Mannesmann Arcor - Internet by Call
#
# based on German Telekom City Call
#   created  97/07/18 by Stefan Markgraf <S_Markgraf@hotmail.com>
#   modified 98/05/01 by Stefan Troeger  <stefan.troeger@wirtschaft.tu-chemnitz.de>
#
# created 98/12/19 by Gregor Eikmeier <gregor.eikmeier@arcormail.de>
#   based on reduction of costs since 98/12/14 
#   look at http://www.arcor.de/tarife/internet.html)
#
#################################################################################

name=German_Arcor_Internet_by_Call
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(0.14, 60)

# Montag bis Freitag
on (monday..friday) between (9:00..17:59)  use (0.14, 60)
on (monday..friday) between (18:00..8:59)  use (0.10, 60) 

# Sonnabend, Sonntag
on (saturday..sunday) between (0:00..23:59) use (0.10, 60)

# Bundeseinheitliche Feiertage
on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between (0:00..23:59) use (0.10, 60)

# Bundeseinheitliche Feiertage:
# 01/01     Neujahr
# easter-2  Karfreitag
# easter    Ostersonntag
# easter+1  Ostermontag
# 05/01     Maifeiertag Tag der Arbeit
# easter+39 Christi Himmelfahrt
# easter+49 Pfingstsonntag
# easter+50 Pfingstmontag
# 10/03     Tag der Deutschen Einheit
# 12/25     1. Weihnachtstag
# 12/26     2. Weihnachtstag
