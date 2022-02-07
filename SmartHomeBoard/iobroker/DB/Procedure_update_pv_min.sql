-- PROCEDURE: public.update_pv_minutes(record)

-- DROP PROCEDURE IF EXISTS public.update_pv_minutes(record);

CREATE OR REPLACE PROCEDURE public.update_pv_minutes(
	rec record)
LANGUAGE 'plpgsql'
AS $BODY$
BEGIN
--raise warning 'Cons=%', rec.consumption_a;
UPDATE public.pv_minute
	SET 
	ts=rec.ts, 
	section=rec.section, 
	day_night=rec.day_night, 
	voltage_a_min=rec.voltage_a_min,
	voltage_b_min=rec.voltage_b_min,
	voltage_c_min=rec.voltage_c_min,
	voltage_a_max=rec.voltage_a_max,
	voltage_b_max=rec.voltage_b_max,
	voltage_c_max=rec.voltage_c_max,
	current_a_max=rec.current_a_max,
	current_b_max=rec.current_b_max,
	current_c_max=rec.current_c_max,
	frequency_a_min=rec.frequency_a_min,
	frequency_b_min=rec.frequency_b_min,
	frequency_c_min=rec.frequency_c_min,
	frequency_a_max=rec.frequency_a_max,
	frequency_b_max=rec.frequency_b_max,
	frequency_c_max=rec.frequency_c_max,
	power_a_max=rec.power_a_max,
	power_b_max=rec.power_b_max,
	power_c_max=rec.power_c_max,
	energy_a_day=rec.energy_a_day,
	energy_b_day=rec.energy_b_day,
	energy_c_day=rec.energy_c_day,
	energy_a_night=rec.energy_a_night,
	energy_b_night=rec.energy_b_night,
	energy_c_night=rec.energy_c_night,
	consumption_a_day=rec.consumption_a_day,
	consumption_b_day=rec.consumption_b_day,
	consumption_c_day=rec.consumption_c_day,
	consumption_a_night=rec.consumption_a_night,
	consumption_b_night=rec.consumption_b_night,
	consumption_c_night=rec.consumption_c_night,
	powerfactor_a_min=rec.powerfactor_a_min,
	powerfactor_b_min=rec.powerfactor_b_min,
	powerfactor_c_min=rec.powerfactor_c_min,
	powerfactor_a_max=rec.powerfactor_a_max,
	powerfactor_b_max=rec.powerfactor_b_max,
	powerfactor_c_max=rec.powerfactor_c_max,
	energy_a=rec.energy_a,
	energy_b=rec.energy_b,
	energy_c=rec.energy_c
	WHERE id=rec.id;
END
$BODY$;
