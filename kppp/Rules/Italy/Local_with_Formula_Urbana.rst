################################################################
#
# Ruleset for Italian Telecom
# based on pppcosts-0.5
#
# written by Mario Weilguni <mweilguni@sime.com>
# 
# "Formula Urbana" by Andrea Rizzi <bilibao@ouverture.it>
#               
# PER USARE QUESTO TIPO DI TARIFFA E' NECESSARIO AVER SOTTOSCRITTO
# IL CONTRATTO TELECOM ITALIA "Formula Urbana"
#
################################################################

name=Italy_Local_Area_with_"Formula_Urbana"
currency_symbol=L.
currency_position=right 
currency_digits=0
flat_init_costs=(151.13,400)

default=(151.13,800)
on (monday..friday) between (8:30..18:30) use(151.13,110)
on (saturday) between(8:00..13:00) use (151.13, 110)