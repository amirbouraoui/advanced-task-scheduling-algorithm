package schedule;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class VerifyInstance {

    public static boolean isValidFile(File file) {
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = reader.readLine()) != null) {
            	// normal regex : ^[0-9]{2,}[[:space:]][0-9]+[[:space:]][0-9]*$
                if (!line.matches("^\\d{2,}\\s\\d+\\s\\d*$")) {	
                    return false;
                }
            }
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
}
