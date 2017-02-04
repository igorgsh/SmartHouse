package sigma.smarthouse.webservice;

/**
 * Created by Igor Shevchenko on 23.01.17  15:36.
 */
public class ControllerMessage {
    private String Id;
    private int OldValue;
    private int NewValue;

    public String getId() {
        return Id;
    }

    public void setId(String id) {
        Id = id;
    }

    public int getOldValue() {
        return OldValue;
    }

    public void setOldValue(int oldValue) {
        OldValue = oldValue;
    }

    public int getNewValue() {
        return NewValue;
    }

    public void setNewValue(int newValue) {
        NewValue = newValue;
    }
}
