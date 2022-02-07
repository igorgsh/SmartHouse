-- PROCEDURE: public.add_pv(timestamp with time zone, timestamp with time zone)

-- DROP PROCEDURE IF EXISTS public.add_pv(timestamp with time zone, timestamp with time zone);

CREATE OR REPLACE PROCEDURE public.add_pv(
	dt_from timestamp with time zone DEFAULT NULL::timestamp with time zone,
	dt_to timestamp with time zone DEFAULT '2100-01-01 02:00:00+02'::timestamp with time zone)
LANGUAGE 'plpgsql'
AS $BODY$
DECLARE
	tsn record;
	prevPV pv_minute;
	recPV pv_minute;
	hr integer;
	ts0 timestamp with time zone;
	ts1 timestamp with time zone;
	sect record;
	isUpdated boolean;
	ts_from bigint;
	ts_to bigint;
	v real;
BEGIN
	IF (dt_from is NULL) THEN
		select max(ts)
		into dt_from
		from pv_minute;
		
		IF (dt_from is NULL) THEN
			dt_from:='2000-01-01 00:00:00+02';
		END IF;
	END IF;
	
	dt_from:=date_trunc('minute', dt_from);
	ts_from := EXTRACT(EPOCH FROM dt_from)*1000;
	ts_to := EXTRACT(EPOCH FROM dt_to)*1000;
	RAISE NOTICE 'Date: from=% to=%', dt_from, dt_to;
	RAISE NOTICE 'TS: from=% to=%', ts_from, ts_to;
	
	FOR sect IN (select distinct section from pv_ids where section in ('Electricity','Heating'))
	LOOP
	
		ts0:='1970-01-01 00:00:00+00';
		isUpdated := false;
		select *
		into prevPV
		from pv_minute
		where ts = (select max(ts) from pv_minute where ts < dt_from) and section=sect.section;
		IF (prevPV.id IS NULL) THEN
			prevPV.energy_a := 0;
			prevPV.energy_b :=0;
			prevPV.energy_c :=0;
			prevPV.energy_a_day := 0;
			prevPV.energy_b_day :=0;
			prevPV.energy_c_day :=0;
			prevPV.energy_a_night := 0;
			prevPV.energy_b_night :=0;
			prevPV.energy_c_night :=0;
		END IF;
		recPV := prevPV;
		
		
		FOR tsn IN 	
				select /*date_trunc('minute', to_timestamp(ts/1000)) as*/ ts,  val, p.section, p.parameter
				from ts_number n
					left join pv_ids p ON n.id=p.number_id   
				where p.section=sect.section
					and val is not null
					and ts between ts_from AND ts_to	
				order by ts
		LOOP
		
			isUpdated := true;
			ts1 := date_trunc('minute', to_timestamp(tsn.ts/1000));
			IF (ts0 != ts1) THEN
				IF (ts0!='1970-01-01 00:00:00+00') THEN
					call update_pv_minutes(recPV);
				END IF;
				prevPV := recPV;
				ts0 := ts1;
				select *
				into recPV
				from pv_minute
				where ts = ts0  and section=sect.section;				
			END IF;
			--raise NOTICE 'TS0=%', ts0;
			--raise NOTICE 'TS=%', ts1;
			IF (recPV.id is null) THEN
				recPV.id = nextval('pv_minute_sequence');
				hr := date_part('hour', ts1);
				IF (hr=23 OR (hr>=0 AND hr<=6)) THEN
					recPV.day_night:=false;
				ELSE
					recPV.day_night:= true;
				END IF;
				recPV.section :=sect.section;
				recPV.ts := ts0;
				recPV.energy_a := prevPV.energy_a;
				recPV.energy_b := prevPV.energy_b;
				recPV.energy_c := prevPV.energy_c;
				recPV.energy_a_day := prevPV.energy_a_day;
				recPV.energy_b_day := prevPV.energy_b_day;
				recPV.energy_c_day := prevPV.energy_c_day;
				recPV.energy_a_night := prevPV.energy_a_night;
				recPV.energy_b_night := prevPV.energy_b_night;
				recPV.energy_c_night := prevPV.energy_c_night;
				recPV.voltage_a_min := (prevPV.voltage_a_min+prevPV.voltage_a_max)/2;
				recPV.voltage_a_max := recPV.voltage_a_min; 
				recPV.voltage_b_min := (prevPV.voltage_b_min+prevPV.voltage_b_max)/2;
				recPV.voltage_b_max := recPV.voltage_b_min; 
				recPV.voltage_c_min := (prevPV.voltage_c_min+prevPV.voltage_c_max)/2;
				recPV.voltage_c_max := recPV.voltage_c_min; 
				recPV.frequency_a_min := (prevPV.frequency_a_min+prevPV.frequency_a_max)/2;
				recPV.frequency_a_max := recPV.frequency_a_min; 
				recPV.frequency_b_min := (prevPV.frequency_b_min+prevPV.frequency_b_max)/2;
				recPV.frequency_b_max := recPV.frequency_b_min; 
				recPV.frequency_c_min := (prevPV.frequency_c_min+prevPV.frequency_c_max)/2;
				recPV.frequency_c_max := recPV.frequency_c_min; 

				INSERT INTO pv_minute SELECT recPV.*;
			END IF;
			--raise warning 'Before EA: val=% parm=% cons=%, prev_en=%, prev_day=%, prev_n=%, en=%, den=%, nen=%',tsn.val, tsn.parameter, recPV.consumption_a, prevPV.energy_a, prevPV.energy_a_day,					prevPV.energy_a_night, recPV.energy_a,recPV.energy_a_day,recPV.energy_a_night;

			CASE
				WHEN tsn.parameter='voltage_a' THEN 
					IF (recPV.voltage_a_min IS NULL OR tsn.val<=recPV.voltage_a_min) THEN 	recPV.voltage_a_min := tsn.val; END IF;
					IF (recPV.voltage_a_max IS NULL OR tsn.val>=recPV.voltage_a_max) THEN 	recPV.voltage_a_max := tsn.val; END IF;
				WHEN tsn.parameter='voltage_b' THEN 
					IF (recPV.voltage_b_min IS NULL OR tsn.val<=recPV.voltage_b_min) THEN 	recPV.voltage_b_min := tsn.val; END IF;
					IF (recPV.voltage_b_max IS NULL OR tsn.val>=recPV.voltage_b_max) THEN 	recPV.voltage_b_max := tsn.val; END IF;
				WHEN tsn.parameter='voltage_c' THEN 
					IF (recPV.voltage_c_min IS NULL OR tsn.val<=recPV.voltage_c_min) THEN 	recPV.voltage_c_min := tsn.val; END IF;
					IF (recPV.voltage_c_max IS NULL OR tsn.val>=recPV.voltage_c_max) THEN 	recPV.voltage_c_max := tsn.val; END IF;

				WHEN tsn.parameter='current_a' THEN 
					IF (recPV.current_a_max IS NULL OR tsn.val>=recPV.current_a_max) THEN 	recPV.current_a_max := tsn.val; END IF;
				WHEN tsn.parameter='current_b' THEN 
					IF (recPV.current_b_max IS NULL OR tsn.val>=recPV.current_b_max) THEN 	recPV.current_b_max := tsn.val; END IF;
				WHEN tsn.parameter='current_c' THEN 
					IF (recPV.current_c_max IS NULL OR tsn.val>=recPV.current_c_max) THEN 	recPV.current_c_max := tsn.val; END IF;

				WHEN tsn.parameter='power_a' THEN 
					IF (recPV.power_a_max IS NULL OR tsn.val>=recPV.power_a_max) THEN 	recPV.power_a_max := tsn.val; END IF;
				WHEN tsn.parameter='power_b' THEN 
					IF (recPV.power_b_max IS NULL OR tsn.val>=recPV.power_b_max) THEN 	recPV.power_b_max := tsn.val; END IF;
				WHEN tsn.parameter='power_c' THEN 
					IF (recPV.power_c_max IS NULL OR tsn.val>=recPV.power_c_max) THEN 	recPV.power_c_max := tsn.val; END IF;

				WHEN tsn.parameter='powerfactor_a' THEN 
					IF (recPV.powerfactor_a_min IS NULL OR tsn.val<=recPV.powerfactor_a_min) THEN 	recPV.powerfactor_a_min := tsn.val; END IF;
					IF (recPV.powerfactor_a_max IS NULL OR tsn.val>=recPV.powerfactor_a_max) THEN 	recPV.powerfactor_a_max := tsn.val; END IF;
				WHEN tsn.parameter='powerfactor_b' THEN 
					IF (recPV.powerfactor_b_min IS NULL OR tsn.val<=recPV.powerfactor_b_min) THEN 	recPV.powerfactor_b_min := tsn.val; END IF;
					IF (recPV.powerfactor_b_max IS NULL OR tsn.val>=recPV.powerfactor_b_max) THEN 	recPV.powerfactor_b_max := tsn.val; END IF;
				WHEN tsn.parameter='powerfactor_c' THEN 
					IF (recPV.powerfactor_c_min IS NULL OR tsn.val<=recPV.powerfactor_c_min) THEN 	recPV.powerfactor_c_min := tsn.val; END IF;
					IF (recPV.powerfactor_c_max IS NULL OR tsn.val>=recPV.powerfactor_c_max) THEN 	recPV.powerfactor_c_max := tsn.val; END IF;

				WHEN tsn.parameter='frequency_a' THEN 
					IF (recPV.frequency_a_min IS NULL OR tsn.val<=recPV.frequency_a_min) THEN 	recPV.frequency_a_min := tsn.val; END IF;
					IF (recPV.frequency_a_max IS NULL OR tsn.val>=recPV.frequency_a_max) THEN 	recPV.frequency_a_max := tsn.val; END IF;
				WHEN tsn.parameter='frequency_b' THEN 
					IF (recPV.frequency_b_min IS NULL OR tsn.val<=recPV.frequency_b_min) THEN 	recPV.frequency_b_min := tsn.val; END IF;
					IF (recPV.frequency_b_max IS NULL OR tsn.val>=recPV.frequency_b_max) THEN 	recPV.frequency_b_max := tsn.val; END IF;
				WHEN tsn.parameter='frequency_c' THEN 
					IF (recPV.frequency_c_min IS NULL OR tsn.val<=recPV.frequency_c_min) THEN 	recPV.frequency_c_min := tsn.val; END IF;
					IF (recPV.frequency_c_max IS NULL OR tsn.val>=recPV.frequency_c_max) THEN 	recPV.frequency_c_max := tsn.val; END IF;

				WHEN tsn.parameter='energy_a' THEN
					v:=tsn.val;
					IF (v IS NULL OR v<prevPV.energy_a) THEN
						v:=prevPV.energy_a;
					END IF;
					recPV.energy_a:=v;
					IF (recPV.day_night) THEN
						recPV.energy_a_day:=v-prevPV.energy_a_night;
					ELSE
						recPV.energy_a_night:=v-prevPV.energy_a_day;										
					END IF;
					recPV.consumption_a_day := recPV.energy_a_day-prevPV.energy_a_day;
					recPV.consumption_a_night := recPV.energy_a_night-prevPV.energy_a_night;
				WHEN tsn.parameter='energy_b' THEN
					v:=tsn.val;
					IF (v IS NULL OR v<prevPV.energy_b) THEN
						v:=prevPV.energy_b;
					END IF;
					recPV.energy_b:=v;
					IF (recPV.day_night) THEN
						recPV.energy_b_day:=v-prevPV.energy_b_night;
					ELSE
						recPV.energy_b_night:=v-prevPV.energy_b_day;										
					END IF;
					recPV.consumption_b_day := recPV.energy_b_day-prevPV.energy_b_day;
					recPV.consumption_b_night := recPV.energy_b_night-prevPV.energy_b_night;
				WHEN tsn.parameter='energy_c' THEN
					v:=tsn.val;
					IF (v IS NULL OR v<prevPV.energy_c) THEN
						v:=prevPV.energy_c;
					END IF;
					recPV.energy_c:=v;
					IF (recPV.day_night) THEN
						recPV.energy_c_day:=v-prevPV.energy_c_night;
					ELSE
						recPV.energy_c_night:=v-prevPV.energy_c_day;										
					END IF;
					recPV.consumption_c_day := recPV.energy_c_day-prevPV.energy_c_day;
					recPV.consumption_c_night := recPV.energy_c_night-prevPV.energy_c_night;
				ELSE raise notice 'Unknown param=%', tsn.parameter;
					
			END CASE;
			--raise warning 'After EA: val=% parm=% cons=%, prev_en=%, prev_day=%, prev_n=%, en=%, den=%, nen=%',tsn.val, tsn.parameter, recPV.consumption_a, prevPV.energy_a, prevPV.energy_a_day,					prevPV.energy_a_night, recPV.energy_a,recPV.energy_a_day,recPV.energy_a_night;
	
		END LOOP;

	IF (isUpdated) THEN
			call update_pv_minutes(recPV);
		END IF;
		
	END LOOP;

END
$BODY$;
