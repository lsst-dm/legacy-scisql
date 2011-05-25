/*
    Copyright (C) 2011 Serge Monkewitz

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License v3 as published
    by the Free Software Foundation, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    A copy of the LGPLv3 is available at <http://www.gnu.org/licenses/>.

    Authors:
        - Serge Monkewitz, IPAC/Caltech

    Work on this project has been sponsored by LSST and SLAC/DOE.
*/

/**
<udf name="dnToFluxSigma" return_type="DOUBLE PRECISION" section="photometry">
    <desc>
        Converts a raw flux error to a calibrated (AB) flux error. The return
        value will be in units of erg/cm<sup>2</sup>/sec/Hz.
    </desc>
    <args>
        <arg name="dn" type="DOUBLE PRECISION" units="DN">
            Raw flux.
        </arg>
        <arg name="dnSigma" type="DOUBLE PRECISION" units="DN">
            Standard deviation of dn.
        </arg>
        <arg name="fluxMag0" type="DOUBLE PRECISION" units="DN">
            Raw flux of a zero-magnitude object.
        </arg>
        <arg name="fluxMag0Sigma" type="DOUBLE PRECISION" units="DN">
            Standard deviation of fluxMag0.
        </arg>
    </args>
    <notes>
        <note>
            All arguments must be convertible to type DOUBLE PRECISION.
        </note>
        <note>
            If any argument is NULL, NaN, or +/-Inf, NULL is returned.
        </note>
        <note>
            If fluxMag0 is zero, NULL is returned.
        </note>
    </notes>
    <example>
        SELECT dnToAbMagSigma(src.psfFlux, src.psfFluxSigma,
                              ccd.fluxMag0, ccd.fluxMag0Sigma)
            FROM Source AS src, Science_Ccd_Exposure ccd
            WHERE src.scienceCcdExposureId = ccd.scienceCcdExposureId
            LIMIT 10;
    </example>
</udf>
*/

#include <stdio.h>

#include "mysql.h"

#include "photometry.h"

#ifdef __cplusplus
extern "C" {
#endif


SCISQL_API my_bool dnToFluxSigma_init(UDF_INIT *initid,
                                      UDF_ARGS *args,
                                      char *message)
{
    size_t i;
    my_bool const_item = 1;
    if (args->arg_count != 4) {
        snprintf(message, MYSQL_ERRMSG_SIZE,
                 "dnToFluxSigma() expects exactly 2 arguments");
        return 1;
    }
    for (i = 0; i < 4; ++i) {
        args->arg_type[i] = REAL_RESULT;
        if (args->args[i] != 0) {
            const_item = 0;
        } 
    }
    initid->maybe_null = 1;
    initid->const_item = const_item;
    initid->decimals = 31;
    return 0;
}


SCISQL_API double dnToFluxSigma(UDF_INIT *initid SCISQL_UNUSED,
                                UDF_ARGS *args,
                                char *is_null,
                                char *error SCISQL_UNUSED)
{
    double **a = (double **) args->args;
    size_t i;
    for (i = 0; i < 4; ++i) {
        if (args->args[i] == 0) {
            *is_null = 1;
            return 0.0;
        }
    }
    return scisql_dn2fluxsigma(*a[0], *a[1], *a[2], *a[3]);
}


#ifdef __cplusplus
} /* extern "C" */
#endif

