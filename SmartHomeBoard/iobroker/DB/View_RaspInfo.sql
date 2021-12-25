-- View: public.RaspInfo

-- DROP VIEW public."RaspInfo";

CREATE OR REPLACE VIEW public."RaspInfo"
 AS
 SELECT "substring"(d.name, length('rpi2.0.'::text) + 1) AS nm,
    n.ts / 1000 AS ts,
    n.val
   FROM datapoints d
     LEFT JOIN ts_number n ON n.id = d.id
  WHERE d.name ~ 'rpi2'::text AND d.type = 0
UNION ALL
 SELECT "substring"(d.name, length('rpi2.0.'::text) + 1) AS nm,
    b.ts / 1000 AS ts,
        CASE
            WHEN b.val THEN 1
            ELSE 0
        END AS val
   FROM datapoints d
     LEFT JOIN ts_bool b ON b.id = d.id AND d.type = 2
  WHERE d.name ~ 'rpi2'::text AND d.type = 2;

ALTER TABLE public."RaspInfo"
    OWNER TO iobroker;

