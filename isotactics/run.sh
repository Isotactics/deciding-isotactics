bin/Isotactics $1 $2 $3 2>/dev/null | dot -Tpng -o $4
gpicview $4 &
