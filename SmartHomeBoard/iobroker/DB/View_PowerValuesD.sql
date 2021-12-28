-- View: public.PowerValuesD

-- DROP VIEW public."PowerValuesD";

CREATE OR REPLACE VIEW public."PowerValuesD"
 AS
 SELECT t.dt AS ts,
    t.section,
    t.day_night,
    avg(t.voltage_a) AS voltage_a,
    avg(t.voltage_b) AS voltage_b,
    avg(t.voltage_c) AS voltage_c,
    avg(t.current_a) AS current_a,
    avg(t.current_b) AS current_b,
    avg(t.current_c) AS current_c,
    avg(t.frequency_a) AS frequency_a,
    avg(t.frequency_b) AS frequency_b,
    avg(t.frequency_c) AS frequency_c,
    avg(t.power_a) AS power_a,
    avg(t.power_b) AS power_b,
    avg(t.power_c) AS power_c,
    max(t.energy_a)-min(t.energy_a) AS energy_a,
    max(t.energy_b)-min(t.energy_b) AS energy_b,
    max(t.energy_c)-min(t.energy_c) AS energy_c,
    avg(t.powerfactor_a) AS powerfactor_a,
    avg(t.powerfactor_b) AS powerfactor_b,
    avg(t.powerfactor_c) AS powerfactor_c
   FROM ( SELECT p.ts,
            date(p.ts) AS dt,
            p.section,
            p.day_night,
            p.voltage_a,
            p.voltage_b,
            p.voltage_c,
            p.current_a,
            p.current_b,
            p.current_c,
            p.frequency_a,
            p.frequency_b,
            p.frequency_c,
            p.power_a,
            p.power_b,
            p.power_c,
            p.energy_a,
            p.energy_b,
            p.energy_c,
            p.powerfactor_a,
            p.powerfactor_b,
            p.powerfactor_c
           FROM "PowerValues" p) t
  GROUP BY t.dt, t.section, t.day_night;

ALTER TABLE public."PowerValuesD"
    OWNER TO iobroker;

