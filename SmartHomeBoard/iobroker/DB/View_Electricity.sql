-- View: public.Electricity

-- DROP VIEW public."Electricity";

CREATE OR REPLACE VIEW public."Electricity"
 AS
 SELECT t2.section,
    "substring"(t2.tt, 0, "position"(t2.tt, '.'::text)) AS category,
    "substring"(t2.tt, "position"(t2.tt, '.'::text) + 1) AS indicator,
    t2.ts,
    t2.val
   FROM ( SELECT "substring"(t1.tp, 0, "position"(t1.tp, '.'::text)) AS section,
            "substring"(t1.tp, "position"(t1.tp, '.'::text) + 1) AS tt,
            t1.ts,
            t1.val
           FROM ( SELECT "substring"(d.name, length('mqtt.0.Others.'::text) + 1) AS tp,
                    to_timestamp((n.ts / 1000)::double precision) AS ts,
                    n.val
                   FROM datapoints d
                     LEFT JOIN ts_number n ON n.id = d.id
                  WHERE d.name ~ 'mqtt.0.Others.Electricity'::text OR d.name ~ 'mqtt.0.Others.Heating'::text) t1) t2;

ALTER TABLE public."Electricity"
    OWNER TO iobroker;

