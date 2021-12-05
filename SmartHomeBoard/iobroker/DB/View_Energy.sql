-- View: public.Energy

-- DROP VIEW public."Energy";

CREATE OR REPLACE VIEW public."Energy"
 AS
 SELECT t2.section,
    "substring"(t2.s2, 0, "position"(t2.s2, '.'::text)) AS category,
    "substring"(t2.s2, "position"(t2.s2, '.'::text) + 1) AS indicator,
    t2.val,
    t2.ts,
    t2.dt,
    t2.hour,
        CASE
            WHEN t2.hour = ANY (ARRAY[23, 0, 1, 2, 3, 4, 5, 6]) THEN false
            ELSE true
        END AS day_night
   FROM ( SELECT t1.s1,
            "substring"(t1.s1, 0, "position"(t1.s1, '.'::text)) AS section,
            "substring"(t1.s1, "position"(t1.s1, '.'::text) + 1) AS s2,
            t1.val,
            t1.ts,
            date(t1.ts) AS dt,
            date_part('hour'::text, t1.ts)::integer AS hour
           FROM ( SELECT "substring"(d.name, length('mqtt.0.Others.'::text) + 1) AS s1,
                    v1.val,
                    to_timestamp((v1.ts / 1000)::double precision) AS ts
                   FROM datapoints d
                     LEFT JOIN ts_number v1 ON d.id = v1.id AND v1.val IS NOT NULL
                  WHERE d.name ~ 'mqtt.0.Others.'::text AND (d.name ~ '.Power'::text OR d.name ~ '.Energy'::text)) t1) t2;

ALTER TABLE public."Energy"
    OWNER TO iobroker;

