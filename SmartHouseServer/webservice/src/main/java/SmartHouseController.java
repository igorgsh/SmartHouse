package sigma.smarthouse.webservice;

import org.springframework.http.MediaType;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import sigma.smarthouse.common.ServiceConfiguration;

/**
 * Created by Igor Shevchenko on 22.01.17.
 */
@RestController
public class SmartHouseController {

    @RequestMapping(value = ServiceConfiguration.HelloUrl , method = RequestMethod.GET, produces = {MediaType.APPLICATION_JSON_VALUE})
    public String hello(@RequestParam(value = "name", defaultValue = "World") String name) {
        return "Hello, " + name;
    }

}
