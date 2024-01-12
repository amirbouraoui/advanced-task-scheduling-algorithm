module java {
	requires javafx.controls;
	requires javafx.fxml;
	requires com.flexganttfx.model;
	requires com.flexganttfx.view;
	exports gui;
	
	opens application to javafx.graphics, javafx.fxml;
	opens gui to javafx.graphics, javafx.fxml;
}
