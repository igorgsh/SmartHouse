package sigma.smarthouse.common;

/**
 * Created by Igor Shevchenko on 22.01.17  23:04.
 */
public class ServiceConfiguration {

    public static final String RootUrl = "/service";

    public static final String HelloUrl = RootUrl + "/hello";
    public static final String MessageUrl = RootUrl + "/message";
    /*
        public static final String MiListUrl = RootUrl + "/miList";
        public static final String QuoteUrl = RootUrl + "/quote";
        public static final String QuoteUrlVar = QuoteUrl + "?miName={miName}&period={period}&begin={begin}&end={end}";
        public static final String MiKeyUrl = RootUrl + "/miKey";
    */
    private String url;


    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }



    public ServiceConfiguration() {

    }

}
