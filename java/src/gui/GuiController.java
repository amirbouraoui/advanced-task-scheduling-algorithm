package gui;

import javafx.scene.layout.AnchorPane;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.RadioButton;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.stage.FileChooser;
import schedule.Machine;
import schedule.TaskFileProcessor;
import schedule.VerifyInstance;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

import com.flexganttfx.model.Layer;
import com.flexganttfx.view.GanttChart;

import application.Scheduling;
import chart.GanttChartFromFile;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;

public class GuiController implements Initializable {
	
	private File inputFile;
	
    @FXML
    private Pane root;
	
    @FXML
    private Button execute_button;

    @FXML
    private AnchorPane ganttChartContainer;

    @FXML
    private ComboBox<String> balance_combo;
    
    @FXML
    private ToggleGroup preemption_group;
    
    @FXML
    private RadioButton pre;
    

    /* ===== Toggle coloring of the gantt chart ===== */
    
    @FXML
    private ToggleButton coloring_toggle_btn;
    
    @FXML
    private void handleColoringToggle() {
        if (coloring_toggle_btn.isSelected()) {
        	coloring_toggle_btn.setText("Enable Coloring");
            coloring_toggle_btn.setStyle("-fx-background-color: white; -fx-text-fill: rgba(0, 0, 0, 0.54);");
            
            coloring_toggle_btn.getStyleClass().add("toggle_noselect_hover");
            coloring_toggle_btn.getStyleClass().remove("toggle_select_hover");
            
            
        } else {
            coloring_toggle_btn.setText("Disable Coloring");
            // Reset any other styles or properties you might have for the button when it is disabled
            coloring_toggle_btn.setStyle(null);
            
            coloring_toggle_btn.getStyleClass().add("toggle_select_hover");
            coloring_toggle_btn.getStyleClass().remove("toggle_noselect_hover");

        }
    }
    
    /* ===== Amount of machines input ===== */
    
    @FXML
    private TextField integerInput;
    
    private void handlePositiveIntegerFilter(KeyEvent event) {
        String currentText = integerInput.getText();
        String newText = currentText + event.getCharacter();

        if (!isValidPositiveIntegerInput(newText)) {
            event.consume();
        }
    }

    private boolean isValidPositiveIntegerInput(String input) {
        return input.matches("^([1-9]|[1-9][0-9]{1,3})$");
    }
    
    private class TextFieldFocusChangeListener implements ChangeListener<Boolean> {
        @Override
        public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
            if (!newValue) { 
                if (integerInput.getText().isEmpty()) {
                	integerInput.setText("1");
                }
            }
        }
    }

    /* ====================================== */
    
    /* ===== Upload Instance Button ===== */
    
    @FXML
    private Button uploadButton;
    
    @FXML
    private void handleUploadClick() {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Choose a file to upload");

        fileChooser.getExtensionFilters().addAll(
                new FileChooser.ExtensionFilter("All Files", "*.*")
        );

        File selectedFile = fileChooser.showOpenDialog(uploadButton.getScene().getWindow());

        if (selectedFile != null) {
            Scene scene = root.getScene();
            if (VerifyInstance.isValidFile(selectedFile)) {
                this.inputFile = selectedFile;
                TaskFileProcessor taskFileProcessor = new TaskFileProcessor(selectedFile);
                int taskAmount = taskFileProcessor.getTaskAmount();
                double taskAVG = taskFileProcessor.getTaskAVG();
                String formattedAVG = String.format("%.2f", taskAVG);
                new ToastNotification("File loaded successully!\nFile name: " + selectedFile.getName() + "\nAmount of Tasks: " + taskAmount + "\nAverage Task duration: " + formattedAVG, scene, Color.LIMEGREEN);
            } else {
                this.inputFile = null;
                new ToastNotification("Invalid file format!", scene, Color.RED);
            }
        }

    }
    
    /* ====================================== */

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        // Add items to ComboBox
        balance_combo.getItems().addAll("Balanced", "Not Balanced");
        balance_combo.getSelectionModel().select(0);

        integerInput.addEventFilter(KeyEvent.KEY_TYPED, this::handlePositiveIntegerFilter);
        integerInput.focusedProperty().addListener(new TextFieldFocusChangeListener());

        // Added handler to trigger execution on Enter key press
        integerInput.setOnAction(event -> {
            if (integerInput.getText().isEmpty()) {
                integerInput.setText("1");
            }
            execute_button_method();
        });

        Machine root = new Machine("Result");
        GanttChart<Machine> gantt = new GanttChart<>(root);
        Layer layer = new Layer("Tasks");
        gantt.getLayers().add(layer);

        AnchorPane.setTopAnchor(gantt, 0.0);
        AnchorPane.setRightAnchor(gantt, 0.0);
        AnchorPane.setBottomAnchor(gantt, 0.0);
        AnchorPane.setLeftAnchor(gantt, 0.0);

        root.setExpanded(true);
        ganttChartContainer.getChildren().add(gantt);
        
        coloring_toggle_btn.setOnAction(event -> handleColoringToggle());
    }
    
    public int execute_button_method() {
        // verify if the upload was successful
        if (this.inputFile == null) {
            Scene scene = root.getScene();
            new ToastNotification("No file found!", scene, Color.ORANGE);
            return 1;
        }

        String selectedAlgorithm = balance_combo.getSelectionModel().getSelectedItem();
        int numberOfMachines = Integer.parseInt(integerInput.getText());
        RadioButton selectedRadioButton = (RadioButton) preemption_group.getSelectedToggle();
        String radioButtonText = selectedRadioButton.getText();
        int preemption = radioButtonText.equals("With Preemption") ? 1 : 0;
        int balanced = selectedAlgorithm.equals("Balanced") ? 1 : 0;

        String instanceInFile = this.inputFile.getAbsolutePath();
        String scheduleOutFile = "./data/" + this.inputFile.getName() + "_output";

        Scheduling result = new Scheduling(instanceInFile, scheduleOutFile, numberOfMachines, preemption, balanced);
        int returnValue = result.getReturnValue();
        
        if(returnValue == 0) {
        	Scene scene = root.getScene();
        	Pane rootPane = (Pane) scene.getRoot();
            rootPane.getChildren().removeIf(node -> "toast-notification".equals(node.getId()));
            try {
                GanttChartFromFile ganttChartFromFile = new GanttChartFromFile();
                ganttChartFromFile.createGanttChart(ganttChartContainer, scheduleOutFile);
            } catch (IOException e) {
            	new ToastNotification("Error reading file: " + e.getMessage(), scene, Color.ORANGE);
                System.out.println("Error reading file: " + e.getMessage());
            }
        }else {
            Scene scene = root.getScene();
            new ToastNotification("Error in scheduling algorithm! (application_Scheduling.c)", scene, Color.RED);
            System.out.println("Error in scheduling algorithm");
        }

        return 0;
    }
}

