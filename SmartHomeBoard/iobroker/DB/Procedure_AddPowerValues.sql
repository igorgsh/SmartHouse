-- PROCEDURE: public.AddPowerValues(bigint, bigint, real)

-- DROP PROCEDURE IF EXISTS public."AddPowerValues"(bigint, bigint, real);

CREATE OR REPLACE PROCEDURE public."AddPowerValues"(
	id_rec bigint,
	ts_rec bigint,
	val_rec real)
LANGUAGE 'plpgsql'
AS $BODY$
DECLARE
	tm_st timestamp;	
	pvId bigint;
	sect text;
	recPV "PowerValues";
	hr integer;
-- Current, Energy, Frequency, PowerFactor, Power, Voltage
-- LineA P0262,P0266
-- LineB P0263,P0265
-- LineС P0267,P0264
	elIds	bigint[] := '{205,209,220, 206,210,221, 207,211,222, 228,219,224, 208,212,225, 223,227,226}';
	htIds	bigint[] := '{184,181,178, 185,182,179, 217,215,213, 187,183,180, 218,215,214, 189,186,188}';

BEGIN
	--raise NOTICE 'START';
	tm_st:=to_timestamp(cast((ts_rec/1000/60) as bigint)*60); -- 1 times per 60 sec
	--raise WARNING '00_%_%_%', id_rec, ts_rec, val_rec;
	CASE 
		WHEN id_rec = ANY(elIds) THEN sect:='Electricity';
		WHEN id_rec = ANY(htIds) THEN sect:='Heating';
		ELSE RETURN ;
	END CASE;
	--raise warning 'Start:%;sect=%', tm_st,sect; 
	select *
	into recPV
	from "PowerValues"
	where section = sect and ts in (select max(ts) from "PowerValues" where ts<=tm_st AND section = sect);
	--raise warning 'POINT0:%; ', recPV; 
	if (recPV.id is NULL) THEN
		recPV.id:=nextval('pv_sequence');
		recPV.ts:=tm_st;
		recPV.section:=sect;
		INSERT INTO "PowerValues" SELECT recPV.*;
	END IF;
	if (recPV.ts != tm_st) THEN
		recPV.id:=nextval('pv_sequence');
		recPV.ts:=tm_st;
		recPV.section:=sect;
		INSERT INTO "PowerValues" SELECT recPV.*;	
	END IF;
	recPV.ts := tm_st;
	recPV.section := sect;
	hr := date_part('hour', recPV.ts);
	IF (hr=23 OR (hr>=0 AND hr<=6)) THEN
		recPV.day_night:=false;
	ELSE
		recPV.day_night:= true;
	END IF;
	
	CASE
		WHEN id_rec = elIds[1] OR id_rec = htIds[1] THEN recPV.current_a := val_rec;
		WHEN id_rec = elIds[2] OR id_rec = htIds[2] THEN recPV.current_b := val_rec;
		WHEN id_rec = elIds[3] OR id_rec = htIds[3] THEN recPV.current_c := val_rec;

		WHEN id_rec = elIds[4] OR id_rec = htIds[4] THEN recPV.energy_a := val_rec;
		WHEN id_rec = elIds[5] OR id_rec = htIds[5] THEN recPV.energy_b := val_rec;
		WHEN id_rec = elIds[6] OR id_rec = htIds[6] THEN recPV.energy_c := val_rec;

		WHEN id_rec = elIds[7] OR id_rec = htIds[7] THEN recPV.frequency_a := val_rec;
		WHEN id_rec = elIds[8] OR id_rec = htIds[8] THEN recPV.frequency_b := val_rec;
		WHEN id_rec = elIds[9] OR id_rec = htIds[9] THEN recPV.frequency_c := val_rec;

		WHEN id_rec = elIds[10]  OR id_rec = htIds[10]  THEN recPV.power_a := val_rec;
		WHEN id_rec = elIds[11] OR id_rec = htIds[11] THEN recPV.power_b := val_rec;
		WHEN id_rec = elIds[12] OR id_rec = htIds[12] THEN recPV.power_c := val_rec;

		WHEN id_rec = elIds[13] OR id_rec = htIds[13] THEN recPV.powerfactor_a := val_rec;
		WHEN id_rec = elIds[14] OR id_rec = htIds[14] THEN recPV.powerfactor_b := val_rec;
		WHEN id_rec = elIds[15] OR id_rec = htIds[15] THEN recPV.powerfactor_c := val_rec;

		WHEN id_rec = elIds[16] OR id_rec = htIds[16]	THEN recPV.voltage_a := val_rec;
		WHEN id_rec = elIds[17] OR id_rec = htIds[17]	THEN recPV.voltage_b := val_rec;
		WHEN id_rec = elIds[18] OR id_rec = htIds[18]	THEN recPV.voltage_c := val_rec;

	END CASE;
	
	UPDATE "PowerValues" 
	SET 
		ts = recPV.ts,
		--section = recPV.section:=sect;
		day_night = recPV.day_night, 
		voltage_a=recPV.voltage_a,
		voltage_b=recPV.voltage_b,
		voltage_c = recPV.voltage_c, 
		
 		current_a = recPV.current_a,
 		current_b = recPV.current_b,
 		current_c = recPV.current_c, 
 		frequency_a = recPV.frequency_a,
 		frequency_b = recPV.frequency_b,
 		frequency_c = recPV.frequency_c, 
		power_a = recPV.power_a,
		power_b = recPV.power_b,
		power_c = recPV.power_c,
		energy_a = recPV.energy_a,
		energy_b = recPV.energy_b,
		energy_c = recPV.energy_c, 
		powerfactor_a = recPV.powerfactor_a,
		powerfactor_b = recPV.powerfactor_b,
		powerfactor_c = recPV.powerfactor_c
		
	WHERE id=recPV.id;
	--raise notice 'POINT1:%;', recPV; 

	RETURN;

END
$BODY$;
