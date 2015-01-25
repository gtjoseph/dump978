// Part of dump978, a UAT decoder.
//
// Copyright 2015, Oliver Jowett <oliver@mutability.co.uk>
//
// This file is free software: you may copy, redistribute and/or modify it  
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 2 of the License, or (at your  
// option) any later version.  
//
// This file is distributed in the hope that it will be useful, but  
// WITHOUT ANY WARRANTY; without even the implied warranty of  
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU  
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License  
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef UAT_H
#define UAT_H

#include <stdint.h>

// A decoded Header element
typedef enum { ADSB_ICAO=0, NATIONAL=1, TISB_ICAO=2, TISB_OTHER=3, VEHICLE=4,
               FIXED_BEACON=5, RESERVED_6=6, RESERVED_7=7 } address_qualifier_t;

struct uat_hdr {
    uint8_t mdb_type;
    address_qualifier_t address_qualifier;
    uint32_t address;
};

typedef enum { BARO, GEO } altitude_type_t;
typedef enum { AIRBORNE_SUBSONIC=0, AIRBORNE_SUPERSONIC=1, GROUND=2, AIRGROUND_RESERVED=3 } airground_state_t;
typedef enum { AIRBORNE_TRACK, GROUND_TRACK, GROUND_MAG_HEADING, GROUND_TRUE_HEADING } track_type_t;

// A decoded State Vector element (TIS-B or ADS-B)
struct uat_sv {
    // validity bits:
    int position_valid : 1;
    int altitude_valid : 1;
    int ns_vel_valid : 1;
    int ew_vel_valid : 1;
    int track_valid : 1;
    int speed_valid : 1;
    int vert_rate_valid : 1;
    int lengthwidth_valid : 1;

    // if position_valid:
    double lat;
    double lon;

    // if altitude_valid:
    altitude_type_t altitude_type;
    int32_t altitude; // in feet
    
    uint8_t nic;

    airground_state_t airground_state;

    // if ns_vel_valid:
    int16_t ns_vel; // in kts
    // if ew_vel_valid:
    int16_t ew_vel; // in kts
    
    // if track_valid:
    track_type_t track_type;
    uint16_t track;

    // if speed_valid:
    uint16_t speed; // in kts

    // if vert_rate_valid:
    int16_t vert_rate; // in ft/min
    altitude_type_t vert_rate_source;

    // if lengthwidth_valid:
    double length; // in meters (just to be different)
    double width;  // in meters (just to be different)
    int position_offset : 1;  // true if Position Offset Applied

    int utc_coupled : 1;      // true if UTC Coupled flag is set (ADS-B)
    uint8_t tisb_site_id;     // TIS-B site ID, or zero in ADS-B messages
};

// A decoded Mode Status element

typedef enum { MAGNETIC, TRUE } heading_type_t;
struct uat_ms {    
    uint8_t emitter_category;
    char callsign[9];
    uint8_t emergency_status;
    uint8_t uat_version;
    uint8_t sil;
    uint8_t transmit_mso;
    uint8_t nac_p;
    uint8_t nac_v;
    uint8_t nic_baro;
  
    // capabilities:
    int has_cdti : 1;
    int has_acas : 1;    
    // operational modes:
    int acas_ra_active : 1;
    int ident_active : 1;
    int atc_services : 1;

    int callsign_id : 1;    
    heading_type_t heading_type;
};

// A decoded Auxiliary Status element

struct uat_auxsv {
    int sec_altitude_valid : 1;
    altitude_type_t sec_altitude_type;
    int32_t sec_altitude; // in feet
};

struct uat_adsb_mdb {
    int sv_valid : 1;
    int ms_valid : 1;
    int auxsv_valid : 1;

    struct uat_hdr hdr;
    struct uat_sv sv;
    struct uat_ms ms;
    struct uat_auxsv auxsv;
};

#endif
