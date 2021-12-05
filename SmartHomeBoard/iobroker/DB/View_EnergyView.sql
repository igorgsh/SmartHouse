-- View: public.EnergyView

-- DROP VIEW public."EnergyView";

CREATE OR REPLACE VIEW public."EnergyView"
 AS
 SELECT v3.source,
    v3.indicator,
    v3.val,
    v3.dt,
    v3.hour::integer AS hour,
        CASE
            WHEN v3.hour = ANY (ARRAY[23::double precision, 0::double precision, 1::double precision, 2::double precision, 3::double precision, 4::double precision, 5::double precision, 6::double precision]) THEN false
            ELSE true
        END AS isday
   FROM ( SELECT v2.source,
            v2.indicator,
            v2.val,
            date(v2.dt) AS dt,
            date_part('hour'::text, v2.dt) AS hour
           FROM ( SELECT "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.'::text) + 1, 5) AS source,
                    "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.P0062.'::text) + 1) AS indicator,
                    btrim(v1.val)::double precision AS val,
                    v1.ts,
                    to_timestamp((v1.ts / 1000)::double precision) AS dt
                   FROM datapoints d
                     LEFT JOIN ts_string v1 ON d.id = v1.id AND v1.val <> '?'::text
                  WHERE d.name ~ 'mqtt.0.Equipment.PowerMeter.'::text AND v1.val IS NOT NULL
                UNION ALL
                 SELECT "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.'::text) + 1, 5) AS source,
                    "substring"(d.name, length('mqtt.0.Equipment.PowerMeter.P0062.'::text) + 1) AS indicator,
                    v1.val::double precision AS val,
                    v1.ts,
                    to_timestamp((v1.ts / 1000)::double precision) AS dt
                   FROM datapoints d
                     LEFT JOIN ts_number v1 ON d.id = v1.id
                  WHERE d.name ~ 'mqtt.0.Equipment.PowerMeter.'::text AND v1.val IS NOT NULL) v2) v3;

ALTER TABLE public."EnergyView"
    OWNER TO iobroker;

