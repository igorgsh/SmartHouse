-- View: public.PowerMeters

-- DROP VIEW public."PowerMeters";

CREATE OR REPLACE VIEW public."PowerMeters"
 AS
 SELECT v1.source,
    v1.indicator,
    v1.val,
    v1.tm,
    date(v1.tm) AS dt,
    date_part('hour'::text, v1.tm)::integer AS hour,
        CASE
            WHEN date_part('hour'::text, v1.tm)::integer = ANY (ARRAY[23, 0, 1, 2, 3, 4, 5, 6]) THEN false
            ELSE true
        END AS day_night,
        CASE
            WHEN v1.source = ANY (ARRAY['P0262'::text, 'P0266'::text]) THEN 'A'::text
            WHEN v1.source = ANY (ARRAY['P0263'::text, 'P0265'::text]) THEN 'B'::text
            WHEN v1.source = ANY (ARRAY['P0267'::text, 'P0264'::text]) THEN 'C'::text
            ELSE NULL::text
        END AS line,
        CASE
            WHEN v1.source = ANY (ARRAY['P0262'::text, 'P0263'::text, 'P0267'::text]) THEN 1
            WHEN v1.source = ANY (ARRAY['P0264'::text, 'P0265'::text, 'P0266'::text]) THEN 2
            ELSE NULL::integer
        END AS grp
   FROM ( SELECT "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.'::text) + 1, 5) AS source,
            "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.P0062.'::text) + 1) AS indicator,
            v1_1.val::double precision AS val,
            v1_1.ts,
            to_timestamp((v1_1.ts / 1000)::double precision) AS tm
           FROM datapoints d
             LEFT JOIN ts_number v1_1 ON d.id = v1_1.id
          WHERE d.name ~ 'mqtt.0.Equipment.PowerMeter.'::text AND v1_1.val IS NOT NULL) v1;

ALTER TABLE public."PowerMeters"
    OWNER TO iobroker;

