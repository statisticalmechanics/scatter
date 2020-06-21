draw delete all
  
[atomselect top "all"] set radius 2

#pbc set {18.882714 18.882714 18.882714 90 90 90} -all
pbc set {5  18.882714 18.882714 90 90 90} -all
pbc box -on  -color red -width 5
