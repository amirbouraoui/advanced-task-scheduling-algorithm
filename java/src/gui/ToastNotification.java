package gui;

import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.effect.DropShadow;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.util.Duration;

public class ToastNotification extends HBox {

	// No magic numbers only constants for better overview
    private static final int ANIMATION_DURATION = 500;
    private static final int TRANSLATION_X = 20;
    private static final double SHADOW_RADIUS = 5.0;
    private static final int SHADOW_OFFSET_X = 2;
    private static final int SHADOW_OFFSET_Y = 2;
    private static final int TOAST_WIDTH_OFFSET = 500;
    private static final Color SHADOW_COLOR = Color.color(0, 0, 0, 0.5);
    private static final int X_HEIGHT_GREEN = 53;
    private static final int X_HEIGHT_DEFAULT = 24;
    
    private Scene scene;

    @SuppressWarnings("exports")
    public ToastNotification(String text, Scene scene, Color status) {
        this.scene = scene;
        setId("toast-notification");

        Label label = createLabel(text);
        Button closeButton = createCloseButton(status);

        getChildren().addAll(label, closeButton);

        configureToastAppearance(status);
        configureToastPosition();

        Pane rootPane = (Pane) scene.getRoot();
        rootPane.getChildren().removeIf(node -> "toast-notification".equals(node.getId()));

        rootPane.getChildren().add(this);

        // Create and play the slide animation
        setupAnimation().play();
    }

    private Label createLabel(String text) {
        Label label = new Label(text);
        label.setFont(Font.font("Roboto", 16));
        label.setTextFill(Color.BLACK);
        return label;
    }

    private Button createCloseButton(Color status) {
        Button closeButton = new Button("x");
        closeButton.setFont(Font.font("Roboto", 18));

        int xHeight = status.equals(Color.LIMEGREEN) ? X_HEIGHT_GREEN : X_HEIGHT_DEFAULT;

        closeButton.setStyle("-fx-background-color: green; -fx-text-fill: black; -fx-padding: -" + xHeight + " -13 3 1; -fx-cursor: hand; -fx-alignment: top-right; ");
        closeButton.setOnAction(e -> closeNotification());
        return closeButton;
    }

    private void configureToastAppearance(Color status) {
        setBackground(new Background(new BackgroundFill(Color.WHITE, new CornerRadii(0, 5 ,5, 0, false), null)));
        setBorder(new Border(new BorderStroke(status, BorderStrokeStyle.SOLID, new CornerRadii(0), new BorderWidths(0, 0, 0, 5))));
        setPadding(new Insets(15, 20, 15, 10));
        setSpacing(10);
        setAlignment(Pos.CENTER_LEFT);

        DropShadow dropShadow = new DropShadow();
        dropShadow.setRadius(SHADOW_RADIUS);
        dropShadow.setOffsetX(SHADOW_OFFSET_X);
        dropShadow.setOffsetY(SHADOW_OFFSET_Y);
        dropShadow.setColor(SHADOW_COLOR);

        setEffect(dropShadow);
    }

    private void configureToastPosition() {
        setTranslateY(20);
        setTranslateX(getWidth() - TOAST_WIDTH_OFFSET);
    }

    private Timeline setupAnimation() {
        Timeline timeline = new Timeline();
        KeyValue kv1 = new KeyValue(translateXProperty(), TRANSLATION_X, Interpolator.EASE_BOTH);
        KeyFrame kf1 = new KeyFrame(Duration.millis(ANIMATION_DURATION), kv1);
        timeline.getKeyFrames().add(kf1);
        return timeline;
    }

    public void closeNotification() {
        ((Pane) scene.getRoot()).getChildren().remove(this);
    }
}

       
