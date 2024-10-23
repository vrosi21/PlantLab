import { bootstrapApplication } from '@angular/platform-browser';
import { appConfig } from './app/app.config'; // Make sure the path is correct
import { AppComponent } from './app/app.component'; // Ensure the path is correct

bootstrapApplication(AppComponent, appConfig).catch((err) =>
  console.error(err)
);
