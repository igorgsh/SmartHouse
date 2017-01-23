package sigma.smarthouse.webservice;

import org.springframework.http.HttpStatus;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import sigma.smarthouse.common.ServiceConfiguration;

import javax.servlet.http.HttpServletRequest;

/**
 * Created by Igor Shevchenko on 22.01.17.
 */
@RestController
public class SmartHouseController {

    @RequestMapping(value = ServiceConfiguration.HelloUrl , method = RequestMethod.GET, produces = {MediaType.APPLICATION_JSON_VALUE})
    public ResponseEntity<String>  hello(HttpServletRequest request, @RequestParam(value = "name", defaultValue = "World") String name) {
        return new ResponseEntity<String>("Hello, " + name, HttpStatus.OK);
    }

    @RequestMapping(value = ServiceConfiguration.MessageUrl, method = RequestMethod.POST, produces = {MediaType.APPLICATION_JSON_UTF8_VALUE})
    public ResponseEntity<ControllerMessage> message(HttpServletRequest request, @RequestBody ControllerMessage message) {

        return new ResponseEntity<ControllerMessage>(message, HttpStatus.OK);
    }

}
