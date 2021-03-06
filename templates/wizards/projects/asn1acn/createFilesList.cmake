######################################################################
# Copyright (C) 2017 N7 Mobile sp. z o. o.
# Contact: http://n7mobile.pl/Space
#
# This file is part of ASN.1/ACN Plugin for QtCreator.
#
# Plugin was developed under a programme and funded by
# European Space Agency.
#
# This Plugin is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This Plugin is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#######################################################################

function(tryCreateFile fileName)
    if (NOT ${ARGC} EQUAL 1)
        return()
    endif()

    if (NOT EXISTS ${fileName})
        file(WRITE ${fileName})
    endif()
endfunction()

function(appendPersistentList)
    set(persistentFiles ${ASN1SCC_PRODUCTS_DIR}/asn1crt.c ${ASN1SCC_PRODUCTS_DIR}/real.c)

    if (${ASN1SCC_ENCODING_OPTIONS} STREQUAL "ACN")
        set(persistentFiles ${persistentFiles} ${ASN1SCC_PRODUCTS_DIR}/acn.c)
    endif()

    foreach(fileName ${persistentFiles})
        tryCreateFile(${fileName})
    endforeach()

    set(ASN1SCC_CPP_PRODUCTS ${ASN1SCC_CPP_PRODUCTS} ${persistentFiles} PARENT_SCOPE)
endfunction()

function(appendVolatileList)
    foreach(fileName ${ASN1ACNSOURCES})
        get_filename_component(extension ${fileName} EXT)
        if (${extension} STREQUAL ".asn" OR ${extension} STREQUAL ".asn1")
            get_filename_component(fileName ${fileName} NAME_WE)
            tryCreateFile(${ASN1SCC_PRODUCTS_DIR}/${fileName}.c)
            set(volatileFiles ${volatileFiles} ${ASN1SCC_PRODUCTS_DIR}/${fileName}.c)
        endif()
    endforeach()

    set(ASN1SCC_CPP_PRODUCTS ${ASN1SCC_CPP_PRODUCTS} ${volatileFiles} PARENT_SCOPE)
endfunction()

if(ASN1ACNSOURCES)
    appendPersistentList()
    appendVolatileList()
endif()
