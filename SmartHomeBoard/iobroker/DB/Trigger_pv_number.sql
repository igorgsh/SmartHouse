-- Trigger: PV_number_trg

-- DROP TRIGGER IF EXISTS "PV_number_trg" ON public.ts_number;

CREATE TRIGGER "PV_number_trg"
    AFTER INSERT OR UPDATE OF val
    ON public.ts_number
    FOR EACH ROW
    WHEN (new.id = ANY (ARRAY[205, 209, 220, 206, 210, 221, 207, 211, 222, 228, 219, 224, 208, 212, 225, 223, 227, 226, 184, 181, 178, 185, 182, 179, 217, 215, 213, 187, 183, 180, 218, 215, 214, 189, 186, 188]))
    EXECUTE FUNCTION public."PowerValue"();