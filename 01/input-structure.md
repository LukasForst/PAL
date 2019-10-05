
---

first line -`R C P K`
* `R` - number of rows
* `C` - number of columns
* `P` - number of vertices with non-zero potential
* `K` - number of spare edges

---

`P` lines of non-zero potential vertices

`r c p` - row, column, potential - vertex at position `(r, c)` has potential `p`

---

`K` lines that are not in the grid

`r`<sub>1</sub> `c`<sub>1</sub> `r`<sub>2</sub> `c`<sub>2</sub> - connection `(r1,c1)` - `(r2,c2)`


---
`nzpv` = non zero potential vertex

Weight `w(u,v)` = distance of `u` from `nzpv` + distance of `v` from `nzpv` + `|` potential of `nzpv_u` - potential of `nzpv_v` `|`