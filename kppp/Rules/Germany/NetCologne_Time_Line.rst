#####################################################################
# German NetCologne rate ruleset
# ------------------------------
# (probably only usefull for anybody living in Cologne)
# This ruleset can be used by NetCologne clients who ordered the package 
# "Time-Line" from NetCologne. For non-NetCologne customers the Telekom
# charge will be higher.
#
# *** for City calls and NetCologne users *** - Use at your own risk.
#     ( monthly package cost : 5,00 DM 			/ 1 month  )
#     ( online charge        : 0,05 DM 			/ 1 minute )
#     ( Telekom charge       : 0,04 DM [8:00..18:00] 	/ 1 minute )
#     (		          or : 0,02 DM [18:00..8:00]+WE	/ 1 minute )
# Only the online and Telekom charges are counted.
#                
# erstellt am 7.5.98 von Roland Pabel (Roland.Pabel@FH-Koeln.de)
#
# Obwohl NetCologne sekundengenau abrechnet, ist es viel einfacher
# in 6 Sekundenschritten abzurechnen. Der errechnete Betrag duerfte
# nur wenig ueber dem Realen liegen. Bisher nicht getestet!
#
# Alle Angaben ohne Gewaehr (besonders nicht von mir :-). 
# Infos zu NetCologne unter http://www.netcologne.de .
#
#####################################################################

name=German_NetCologne_Time-Line
currency_symbol=DM
currency_position=right 
currency_digits=2
per_connection=0.0

# default : Cost of 6 seconds in the time periods (Monday..Friday, 8:00..18:00)
default=(0.009, 6)

# (other time periods / Weekend)
on (monday..friday)   between (18:00..8:00)  use (0.007, 6)
on (saturday..sunday) between (00:00..23:59) use (0.007, 6)

# Spezialtarif an bundeseinheitlichen Feiertagen
# Feiertagstarif = Wochenendtarif

on (01/01, easter-2, easter, easter+1, 05/01, easter+39, easter+49, easter+50, 10/03, 12/25, 12/26) between (00:00..23:59) use (0.007, 6)

# bundeseinheitliche Feiertage:
# 01/01     Neujahr
# easter-2  Karfreitag
# easter    Ostersonntag
# easter+1  Ostermontag
# 05/01     Maifeiertag Tag der Arbeit
# easter+39 Christi Himmelfahrt
# easter+49 Pfingstsonntag
# easter+50 Pfingstmontag
# 10/03     Tag der Deutschen Einheit
# 12/25     Weihnachten
# 12/26     Weihnachten

# Feiertage in *** Kvln ***

on (easter-48, easter-52) between (00:00..23:59) use (0.007, 6)

# NetCologne bietet den Feiertagstarif auch an diesen Tagen an :
# easter-48 Rosenmontag 
# easter-52 Weiberfastnacht
