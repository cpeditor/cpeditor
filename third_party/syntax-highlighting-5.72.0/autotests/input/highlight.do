/* Test file for kate's stata syntax highlighting
*/
*! version 1.2.0 2mar2003 E. Leuven
program define spellsplit
        version 7
        syntax [anything], spell(varlist min=2 max=2) [ by(varlist)]
        tokenize `spell'
        local date0 `1'
        local date1 `2'
        local f0 : format `date0'
        local f1 : format `date1'

        /* set default statistic */
        local current "mean"

        gettoken left anything : anything, match(prns)
        while "`left'"!="" {
                if "`prns'"!="" {
                        if !inlist("`left'","mean","sum") {
                                di as error "Statistic `left' not supported"
                                exit 198
                        }
                        local current "`left'"
                }
                else {
                        local `current'vars  ``current'vars' `left'
                }
                gettoken left anything : anything, match(prns)
        }
        if ("`meanvars'"!="") {
                confirm var `meanvars'
                unab meanvars : `meanvars'
        }
        if ("`sumvars'"!="") {
                confirm var `sumvars'
                unab sumvars : `sumvars'
        }

        quietly {
                g _count = 1
                local xvars `meanvars' `sumvars' _count

                /* create dummy by-var if no by option is specified */
                if "`by'"=="" {
                        tempvar by
                        g byte `by' = 1
                }
                tempvar `xvars' `by'

                /* create negative for subtraction when spell ends */
                cap foreach v of varlist `xvars' {
                        g double ``v'' = -`v'
                        local txvars `txvars' ``v''
                }
                cap foreach v of varlist `by' {
                        g double ``v'' = `v'
                        local txvars `txvars' ``v''
                }

                stack `date0' `xvars' `by' `date1' `txvars', into(`date0' `xvars' `by') clear

                /* calculate totals per date */
                cap foreach v of varlist `xvars' {
                        egen double ``v'' = sum(`v'), by(`by' `date0')
                }

                /* unique dates only */
                by `by' `date0', sort: keep if _n==1

                /* calculate totals (+ when spell starts - when ends) */
                sort `by'
                cap foreach v of varlist `xvars' {
                        by `by': replace `v' = sum(``v'')
                }
                by `by': g `date1' = `date0'[_n + 1]

                drop if `date0'>`date1'
                drop _stack
                drop if _count==0
                order `by' `date0' `date1' `xvars'
                format `date0' `f0'
                format `date1' `f1'

                cap for var `meanvars': replace X = X/_count

                compress
        }

end