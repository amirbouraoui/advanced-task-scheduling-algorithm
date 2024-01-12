package application;
	
import java.io.IOException;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import com.flexganttfx.core.FlexGanttFX;


public class Main extends Application {
    @Override
    public void start(Stage primaryStage) {
        try {
            setupUI(primaryStage);
        } catch(IOException e) {
            System.out.println("Error loading FXML file: " + e.getMessage());
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    private void setupUI(Stage primaryStage) throws IOException {
        Parent root = FXMLLoader.load(getClass().getResource("/gui/gui.fxml"));
        Scene scene = new Scene(root, 950, 550);
        
        scene.getStylesheets().add(getClass().getResource("application.css").toExternalForm());
        
        primaryStage.setTitle("Scheduling - Interface");
        primaryStage.getIcons().add(new Image(getClass().getResourceAsStream("/assets/scheduling_icon.png")));
        primaryStage.setScene(scene);
        primaryStage.setResizable(false);
        primaryStage.show();
    }

    public static void main(String[] args) {
    	FlexGanttFX.setLicenseKey("LIC=UniversiteDeLorraine;VEN=DLSC;VER=12;PRO=STANDARD;RUN=yes;CTR=1;SignCode=3F;Signature=302C021407AA0273FB89085A29ED2E197A7A58B63D35F4B402141ED5C7AB46ED031A2DDFFFE3454DD60BC5973048");
        launch(args);
    }
}
